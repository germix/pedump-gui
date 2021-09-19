#include "TreeItemImportsSection.h"

//
// Dump the imports table (the .idata section) of a PE file
//
QString DumpImportsSection(DWORD base, PIMAGE_NT_HEADERS pNTHeader)
{
	QString str;

	PIMAGE_IMPORT_DESCRIPTOR importDesc;
	PIMAGE_SECTION_HEADER pSection;
	PIMAGE_THUNK_DATA thunk, thunkIAT=0;
	PIMAGE_IMPORT_BY_NAME pOrdinalName;
	DWORD importsStartRVA;
	PSTR pszTimeDate;

	// Look up where the imports section is (normally in the .idata section)
	// but not necessarily so.  Therefore, grab the RVA from the data dir.
	importsStartRVA = GetImgDirEntryRVA(pNTHeader,IMAGE_DIRECTORY_ENTRY_IMPORT);
	if(!importsStartRVA)
		return str;

	// Get the IMAGE_SECTION_HEADER that contains the imports.  This is
	// usually the .idata section, but doesn't have to be.
	pSection = GetEnclosingSectionHeader( importsStartRVA, pNTHeader );
	if(!pSection)
		return str;

	importDesc = (PIMAGE_IMPORT_DESCRIPTOR)GetPtrFromRVA(importsStartRVA,pNTHeader,base);
	if(!importDesc)
		return str;

	str += QString().sprintf("Imports Table:\n\n");

	while(1)
	{
		// See if we've reached an empty IMAGE_IMPORT_DESCRIPTOR
		if((importDesc->TimeDateStamp==0 ) && (importDesc->Name == 0))
			break;

		str += QString().sprintf("  %s\n",
				GetPtrFromRVA(importDesc->Name, pNTHeader, base));

		str += QString().sprintf("  OrigFirstThunk:  %08X (Unbound IAT)\n",
				importDesc->Characteristics);

		pszTimeDate = ctime((time_t *)&importDesc->TimeDateStamp);
		str += QString().sprintf("  TimeDateStamp:   %08X", importDesc->TimeDateStamp);
		str += QString().sprintf( pszTimeDate ?  " -> %s" : "\n", pszTimeDate );

		str += QString().sprintf("  ForwarderChain:  %08X\n", importDesc->ForwarderChain);
		str += QString().sprintf("  First thunk RVA: %08X\n", importDesc->FirstThunk);

		thunk = (PIMAGE_THUNK_DATA)importDesc->Characteristics;
		thunkIAT = (PIMAGE_THUNK_DATA)importDesc->FirstThunk;

		if(thunk == 0) // No Characteristics field?
		{
			// Yes! Gotta have a non-zero FirstThunk field then.
			thunk = thunkIAT;

			if(thunk == 0) // No FirstThunk field?  Ooops!!!
				return str;
		}

		// Adjust the pointer to point where the tables are in the
		// mem mapped file.
		thunk = (PIMAGE_THUNK_DATA)GetPtrFromRVA((DWORD)thunk, pNTHeader, base);
		if(!thunk)
			return str;

		thunkIAT = (PIMAGE_THUNK_DATA)GetPtrFromRVA((DWORD)thunkIAT, pNTHeader, base);

		str += QString().sprintf("  Ordn  Name\n");

		while(1) // Loop forever (or until we break out)
		{
			if(thunk->u1.AddressOfData == 0)
				break;

			if(thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)
			{
				str += QString().sprintf("  %4u", IMAGE_ORDINAL(thunk->u1.Ordinal));
			}
			else
			{
				pOrdinalName = (PIMAGE_IMPORT_BY_NAME)GetPtrFromRVA(
							thunk->u1.AddressOfData,
							pNTHeader,
							base);

				str += QString().sprintf("  %4u  %s", pOrdinalName->Hint, pOrdinalName->Name);
			}

			if(importDesc->TimeDateStamp)
			{
				str += QString().sprintf(" (Bound to: %08X)", thunkIAT->u1.Function);
			}

			str += QString().sprintf("\n");

			thunk++;			// Advance to next thunk
			thunkIAT++;			// advance to next thunk
		}

		importDesc++; // advance to next IMAGE_IMPORT_DESCRIPTOR

		str += QString().sprintf("\n");
	}

	return str;
}

QWidget* TreeItemImportsSection::getWidget() const
{
	return createTextEdit(DumpImportsSection(base, pNTHeader));
}
