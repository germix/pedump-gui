#include "TreeItemResourceSection.h"

extern PIMAGE_RESOURCE_DIRECTORY_ENTRY pStrResEntries;
extern PIMAGE_RESOURCE_DIRECTORY_ENTRY pDlgResEntries;
extern DWORD cStrResEntries;
extern DWORD cDlgResEntries;

// Function prototype (necessary because two functions recurse)
QString DumpResourceDirectory(
	PIMAGE_RESOURCE_DIRECTORY resDir, DWORD resourceBase,
	DWORD level, DWORD resourceType
);

// The predefined resource types
const char* SzResourceTypes[] =
{
	"???_0",
	"CURSOR",
	"BITMAP",
	"ICON",
	"MENU",
	"DIALOG",
	"STRING",
	"FONTDIR",
	"FONT",
	"ACCELERATORS",
	"RCDATA",
	"MESSAGETABLE",
	"GROUP_CURSOR",
	"???_13",
	"GROUP_ICON",
	"???_15",
	"VERSION",
	"DLGINCLUDE",
	"???_18",
	"PLUGPLAY",
	"VXD",
	"ANICURSOR",
	"ANIICON",
};

PIMAGE_RESOURCE_DIRECTORY_ENTRY pStrResEntries = 0;
PIMAGE_RESOURCE_DIRECTORY_ENTRY pDlgResEntries = 0;
DWORD cStrResEntries = 0;
DWORD cDlgResEntries = 0;

DWORD GetOffsetToDataFromResEntry(
	DWORD base,
	DWORD resourceBase,
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResEntry)
{
	// The IMAGE_RESOURCE_DIRECTORY_ENTRY is gonna point to a single
	// IMAGE_RESOURCE_DIRECTORY, which in turn will point to the
	// IMAGE_RESOURCE_DIRECTORY_ENTRY, which in turn will point
	// to the IMAGE_RESOURCE_DATA_ENTRY that we're really after.  In
	// other words, traverse down a level.

	PIMAGE_RESOURCE_DIRECTORY pStupidResDir;
	pStupidResDir = (PIMAGE_RESOURCE_DIRECTORY)
					(resourceBase + pResEntry->OffsetToDirectory);

	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResDirEntry =
			(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pStupidResDir + 1);// PTR MATH

	PIMAGE_RESOURCE_DATA_ENTRY pResDataEntry =
			(PIMAGE_RESOURCE_DATA_ENTRY)(resourceBase +
										 pResDirEntry->OffsetToData);

	return pResDataEntry->OffsetToData;
}

QString DumpStringTable(
	DWORD base,
	PIMAGE_NT_HEADERS pNTHeader,
	DWORD resourceBase,
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pStrResEntry,
	DWORD cStrResEntries)
{
	QString str;

	for(unsigned int i = 0; i < cStrResEntries; i++, pStrResEntry++)
	{
		DWORD offsetToData = GetOffsetToDataFromResEntry( base, resourceBase, pStrResEntry);

		PWORD pStrEntry = (PWORD)GetPtrFromRVA(offsetToData, pNTHeader, base);
		if(!pStrEntry)
			break;

		unsigned int id = (pStrResEntry->Name - 1) << 4;

		for(unsigned int j = 0; j < 16; j++)
		{
			WORD len = *pStrEntry++;
			if(len)
			{
				str += QString().sprintf("%-5u: ", id + j);

				for(unsigned int k = 0; k < qMin(len, (WORD)64); k++)
				{
					const char* s;
					char szBuff[20];
					char c = (char)pStrEntry[k];
					switch(c)
					{
						case '\t': s = "\\t"; break;
						case '\r': s = "\\r"; break;
						case '\n': s = "\\n"; break;
						default:
							sprintf( szBuff, "%c", isprint(c) ? c : '.' );
							s = szBuff;
							break;
					}

					str += QString().sprintf( s );
				}

				str += QString().sprintf("\n");
			}

			pStrEntry += len;
		}
	}
	return str;
}

QString DumpDialogs(
	DWORD base,
	PIMAGE_NT_HEADERS pNTHeader,
	DWORD resourceBase,
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pDlgResEntry,
	DWORD cDlgResEntries)
{
	QString str;

	for(unsigned int i = 0; i < cDlgResEntries; i++, pDlgResEntry++)
	{
		DWORD offsetToData = GetOffsetToDataFromResEntry( base, resourceBase, pDlgResEntry );

		PDWORD pDlgStyle = (PDWORD)GetPtrFromRVA(offsetToData, pNTHeader, base);
		if(!pDlgStyle)
			break;

		str += QString().sprintf("  ====================\n");
		if(HIWORD(*pDlgStyle) != 0xFFFF)
		{
			//	A regular DLGTEMPLATE
			DLGTEMPLATE* pDlgTemplate = (DLGTEMPLATE*)pDlgStyle;

			str += QString().sprintf("  style: %08X\n", pDlgTemplate->style);
			str += QString().sprintf("  extended style: %08X\n", pDlgTemplate->dwExtendedStyle);

			str += QString().sprintf("  controls: %u\n", pDlgTemplate->cdit);
			str += QString().sprintf("  (%u,%u) - (%u,%u)\n",
						pDlgTemplate->x, pDlgTemplate->y,
						pDlgTemplate->x + pDlgTemplate->cx,
						pDlgTemplate->y + pDlgTemplate->cy);

			PWORD pMenu = (PWORD)(pDlgTemplate + 1); // ptr math!

			//
			// First comes the menu
			//
			if(*pMenu)
			{
				if(0xFFFF == *pMenu)
				{
					pMenu++;
					str += QString().sprintf("  ordinal menu: %u\n", *pMenu);
				}
				else
				{
					str += QString().sprintf("  menu: ");
					while(*pMenu)
					{
						str += QString().sprintf("%c", LOBYTE(*pMenu++));
					}

					pMenu++;
					str += QString().sprintf("\n");
				}
			}
			else
			{
				pMenu++; // Advance past the menu name
			}

			//
			// Next comes the class
			//
			PWORD pClass = pMenu;

			if(*pClass)
			{
				if(0xFFFF == *pClass)
				{
					pClass++;
					str += QString().sprintf("  ordinal class: %u\n", *pClass);
				}
				else
				{
					str += QString().sprintf("  class: ");
					while(*pClass)
					{
						str += QString().sprintf("%c", LOBYTE(*pClass++));
					}
					pClass++;
					str += QString().sprintf("\n");
				}
			}
			else
			{
				pClass++; // Advance past the class name
			}

			//
			// Finally comes the title
			//

			PWORD pTitle = pClass;
			if(*pTitle)
			{
				str += QString().sprintf("  title: ");

				while(*pTitle)
				{
					str += QString().sprintf("%c", LOBYTE(*pTitle++));
				}

				pTitle++;
			}
			else
			{
				pTitle++; // Advance past the Title name
			}

			str += QString().sprintf("\n");

			PWORD pFont = pTitle;

			if(pDlgTemplate->style & DS_SETFONT)
			{
				str += QString().sprintf("  Font: %u point ", *pFont++);
				while(*pFont)
				{
					str += QString().sprintf("%c", LOBYTE(*pFont++));
				}

				pFont++;
				str += QString().sprintf("\n");
			}
			else
			{
				pFont = pTitle;
			}

			// DLGITEMPLATE starts on a 4 byte boundary
			LPDLGITEMTEMPLATE pDlgItemTemplate = (LPDLGITEMTEMPLATE)pFont;

			for(unsigned int i = 0; i < pDlgTemplate->cdit; i++)
			{
				// Control item header....
				pDlgItemTemplate = (DLGITEMTEMPLATE*)(((DWORD)pDlgItemTemplate+3) & ~3);

				str += QString().sprintf("    style: %08X\n", pDlgItemTemplate->style);
				str += QString().sprintf("    extended style: %08X\n", pDlgItemTemplate->dwExtendedStyle);

				str += QString().sprintf("    (%u,%u) - (%u,%u)\n",
							pDlgItemTemplate->x, pDlgItemTemplate->y,
							pDlgItemTemplate->x + pDlgItemTemplate->cx,
							pDlgItemTemplate->y + pDlgItemTemplate->cy);
				str += QString().sprintf("    id: %u\n", pDlgItemTemplate->id);

				//
				// Next comes the control's class name or ID
				//
				PWORD pClass = (PWORD)(pDlgItemTemplate + 1);
				if(*pClass)
				{
					if(0xFFFF == *pClass)
					{
						pClass++;
						str += QString().sprintf("    ordinal class: %u", *pClass++);
					}
					else
					{
						str += QString().sprintf("    class: ");
						while(*pClass)
						{
							str += QString().sprintf("%c", LOBYTE(*pClass++));
						}

						pClass++;
						str += QString().sprintf("\n");
					}
				}
				else
				{
					pClass++;
				}

				str += QString().sprintf("\n");

				//
				// next comes the title
				//

				PWORD pTitle = pClass;

				if(*pTitle)
				{
					str += QString().sprintf("    title: ");
					if(0xFFFF == *pTitle)
					{
						pTitle++;
						str += QString().sprintf("%u\n", *pTitle++);
					}
					else
					{
						while(*pTitle)
						{
							str += QString().sprintf("%c", LOBYTE(*pTitle++));
						}
						pTitle++;
						str += QString().sprintf("\n");
					}
				}
				else
				{
					pTitle++; // Advance past the Title name
				}

				str += QString().sprintf("\n");

				PBYTE pCreationData = (PBYTE)(((DWORD)pTitle + 1) & 0xFFFFFFFE);

				if(*pCreationData)
					pCreationData += *pCreationData;
				else
					pCreationData++;

				pDlgItemTemplate = (DLGITEMTEMPLATE*)pCreationData;

				str += QString().sprintf("\n");
			}

			str += QString().sprintf("\n");
		}
		else
		{
			// A DLGTEMPLATEEX
		}

		str += QString().sprintf("\n");
	}

	return str;
}

// Get an ASCII string representing a resource type
void GetResourceTypeName(DWORD type, PSTR buffer, UINT cBytes)
{
	if(type <= (WORD)(uintptr_t)RT_ANIICON)
		strncpy(buffer, SzResourceTypes[type], cBytes);
	else
		sprintf(buffer, "%X", type);
}

//
// If a resource entry has a string name (rather than an ID), go find
// the string and convert it from unicode to ascii.
//
void GetResourceNameFromId
(
	DWORD id,
	DWORD resourceBase,
	PSTR buffer,
	UINT cBytes)
{
	PIMAGE_RESOURCE_DIR_STRING_U prdsu;

	// If it's a regular ID, just format it.
	if(!(id & IMAGE_RESOURCE_NAME_IS_STRING))
	{
		sprintf(buffer, "%X", id);
		return;
	}

	id &= 0x7FFFFFFF;
	prdsu = (PIMAGE_RESOURCE_DIR_STRING_U)(resourceBase + id);

	// prdsu->Length is the number of unicode characters
	WideCharToMultiByte(CP_ACP, 0, prdsu->NameString, prdsu->Length, buffer, cBytes, 0, 0);
	buffer[ qMin(cBytes-1, UINT(prdsu->Length)) ] = 0;  // Null terminate it!!!
}

//
// Dump the information about one resource directory entry.  If the
// entry is for a subdirectory, call the directory dumping routine
// instead of printing information in this routine.
//
QString DumpResourceEntry
(
	PIMAGE_RESOURCE_DIRECTORY_ENTRY resDirEntry,
	DWORD resourceBase,
	DWORD level)
{
	QString str;
	UINT i;
	char nameBuffer[128];
	PIMAGE_RESOURCE_DATA_ENTRY pResDataEntry;

	if(resDirEntry->OffsetToData & IMAGE_RESOURCE_DATA_IS_DIRECTORY)
	{
		str += DumpResourceDirectory(
			(PIMAGE_RESOURCE_DIRECTORY)((resDirEntry->OffsetToData & 0x7FFFFFFF) + resourceBase),
			resourceBase,
			level,
			resDirEntry->Name);
		return str;
	}

	// Spit out the spacing for the level indentation
	for(i = 0; i < level; i++)
		str += QString().sprintf("    ");

	if(resDirEntry->Name & IMAGE_RESOURCE_NAME_IS_STRING)
	{
		GetResourceNameFromId(resDirEntry->Name, resourceBase, nameBuffer, sizeof(nameBuffer));
		str += QString().sprintf("Name: %s  DataEntryOffs: %08X\n", nameBuffer, resDirEntry->OffsetToData);
	}
	else
	{
		str += QString().sprintf("ID: %08X  DataEntryOffs: %08X\n", resDirEntry->Name, resDirEntry->OffsetToData);
	}

	// the resDirEntry->OffsetToData is a pointer to an
	// IMAGE_RESOURCE_DATA_ENTRY.  Go dump out that information.  First,
	// spit out the proper indentation
	for(i = 0; i < level; i++)
		str += QString().sprintf("    ");

	pResDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)(resourceBase + resDirEntry->OffsetToData);
	str += QString().sprintf("DataRVA: %05X  DataSize: %05X  CodePage: %X\n",
			pResDataEntry->OffsetToData,
			pResDataEntry->Size,
			pResDataEntry->CodePage);

	return str;
}

//
// Dump the information about one resource directory.
//
QString DumpResourceDirectory
(
	PIMAGE_RESOURCE_DIRECTORY resDir,
	DWORD resourceBase,
	DWORD level,
	DWORD resourceType
)
{
	QString str;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY resDirEntry;
	char szType[64];
	UINT i;

	// Level 1 resources are the resource types
	if(level == 1)
	{
		str += QString().sprintf("    --------------------------------------------------------------\n" );

		if(resourceType & IMAGE_RESOURCE_NAME_IS_STRING)
		{
			GetResourceNameFromId(resourceType, resourceBase, szType, sizeof(szType));
		}
		else
		{
			GetResourceTypeName( resourceType, szType, sizeof(szType));
		}
	}
	else // All other levels, just print out the regular id or name
	{
		GetResourceNameFromId(resourceType, resourceBase, szType, sizeof(szType));
	}

	// Spit out the spacing for the level indentation
	for(i = 0; i < level; i++)
		str += QString().sprintf("    ");

	str += QString().sprintf(
		"ResDir (%s) Entries:%02X (Named:%02X, ID:%02X) TimeDate:%08X",
		szType,
		resDir->NumberOfNamedEntries + resDir->NumberOfIdEntries,
		resDir->NumberOfNamedEntries,
		resDir->NumberOfIdEntries,
		resDir->TimeDateStamp);

	if(resDir->MajorVersion || resDir->MinorVersion)
		str += QString().sprintf(" Vers:%u.%02u", resDir->MajorVersion, resDir->MinorVersion);
	if(resDir->Characteristics)
		str += QString().sprintf(" Char:%08X", resDir->Characteristics);
	str += QString().sprintf("\n");

	//
	// The "directory entries" immediately follow the directory in memory
	//
	resDirEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(resDir+1);

	// If it's a stringtable, save off info for future use
	if(level == 1 && (resourceType == (WORD)(uintptr_t)RT_STRING))
	{
		pStrResEntries = resDirEntry;
		cStrResEntries = resDir->NumberOfIdEntries;
	}

	// If it's a stringtable, save off info for future use
	if(level == 1 && (resourceType == (WORD)(uintptr_t)RT_DIALOG))
	{
		pDlgResEntries = resDirEntry;
		cDlgResEntries = resDir->NumberOfIdEntries;
	}

	for(i = 0; i < resDir->NumberOfNamedEntries; i++, resDirEntry++)
		str += DumpResourceEntry(resDirEntry, resourceBase, level+1);

	for(i = 0; i < resDir->NumberOfIdEntries; i++, resDirEntry++)
		str += DumpResourceEntry(resDirEntry, resourceBase, level+1);

	return str;
}

//
// Top level routine called to dump out the entire resource hierarchy
//
QString DumpResourceSection(DWORD base, PIMAGE_NT_HEADERS pNTHeader)
{
	QString str;
	DWORD resourcesRVA;
	PIMAGE_RESOURCE_DIRECTORY resDir;

	resourcesRVA = GetImgDirEntryRVA(pNTHeader, IMAGE_DIRECTORY_ENTRY_RESOURCE);
	if(!resourcesRVA)
		return str;

	resDir = (PIMAGE_RESOURCE_DIRECTORY)GetPtrFromRVA(resourcesRVA, pNTHeader, base);

	if(!resDir)
		return str;

	str += QString().sprintf("Resources (RVA: %X)\n", resourcesRVA );

	str += DumpResourceDirectory(resDir, (DWORD)resDir, 0, 0);

	str += "\n";

	if(cStrResEntries)
	{
		str += "String Table\n";

		str += DumpStringTable(base, pNTHeader, (DWORD)resDir, pStrResEntries, cStrResEntries);

		str += "\n";
	}

	if(cDlgResEntries)
	{
		str += "Dialogs\n" ;

		str += DumpDialogs(base, pNTHeader, (DWORD)resDir, pDlgResEntries, cDlgResEntries);

		str += "\n";
	}
	return str;
}

QWidget* TreeItemResourceSection::getWidget() const
{
	return createTextEdit(DumpResourceSection(base, pNTHeader));
}
