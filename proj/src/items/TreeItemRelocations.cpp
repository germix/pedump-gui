#include "TreeItemRelocations.h"

typedef struct _i386RelocTypes
{
	WORD type;
	PCSTR name;
} i386RelocTypes;

// ASCII names for the various relocations used in i386 COFF OBJs
const i386RelocTypes i386Relocations[] =
{
	{ IMAGE_REL_I386_ABSOLUTE, "ABSOLUTE" },
	{ IMAGE_REL_I386_DIR16, "DIR16" },
	{ IMAGE_REL_I386_REL16, "REL16" },
	{ IMAGE_REL_I386_DIR32, "DIR32" },
	{ IMAGE_REL_I386_DIR32NB, "DIR32NB" },
	{ IMAGE_REL_I386_SEG12, "SEG12" },
	{ IMAGE_REL_I386_SECTION, "SECTION" },
	{ IMAGE_REL_I386_SECREL, "SECREL" },
	{ IMAGE_REL_I386_REL32, "REL32" },
};
#define I386RELOCTYPECOUNT (sizeof(i386Relocations) / sizeof(i386RelocTypes))

//
// Given an i386 OBJ relocation type, return its ASCII name in a buffer
//
void GetObjRelocationName(WORD type, PSTR buffer, DWORD cBytes)
{
	for(DWORD i = 0; i < I386RELOCTYPECOUNT; i++)
	{
		if(type == i386Relocations[i].type)
		{
			strncpy(buffer, i386Relocations[i].name, cBytes);
			return;
		}
	}
	sprintf( buffer, "???_%X", type);
}

//
// Dump the relocation table for one COFF section
//
QString DumpObjRelocations(PIMAGE_RELOCATION pRelocs, DWORD count)
{
	QString str;
	char szTypeName[32];

	for(DWORD i = 0; i < count; i++)
	{
		GetObjRelocationName(pRelocs->Type, szTypeName, sizeof(szTypeName));
		str += QString().sprintf("  Address: %08X  SymIndex: %08X  Type: %s\n",
				pRelocs->VirtualAddress,
				pRelocs->SymbolTableIndex,
				szTypeName);
		pRelocs++;
	}

	return str;
}

QString DumpRelocations(PIMAGE_FILE_HEADER pImageFileHeader, PIMAGE_SECTION_HEADER pSections)
{
	QString str;

	for( int i = 0; i < pImageFileHeader->NumberOfSections; i++)
	{
		if(pSections[i].PointerToRelocations == 0)
			continue;

		str += QString().sprintf("Section %02X (%.8s) relocations\n", i, pSections[i].Name);

		str += DumpObjRelocations(
				MakePtr(PIMAGE_RELOCATION, pImageFileHeader, pSections[i].PointerToRelocations),
				pSections[i].NumberOfRelocations );

		str += "\n";
	}

	return str;
}

QWidget* TreeItemRelocations::getWidget() const
{
	return createTextEdit(DumpRelocations(pImageFileHeader, pSections));
}
