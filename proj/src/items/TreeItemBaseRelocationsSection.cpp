#include "TreeItemBaseRelocationsSection.h"

// The names of the available base relocations
const char* SzRelocTypes[] =
{
	"ABSOLUTE",
	"HIGH",
	"LOW",
	"HIGHLOW",
	"HIGHADJ",
	"MIPS_JMPADDR",
	"SECTION",
	"REL32",
};

//
// Dump the base relocation table of a PE file
//
QString DumpBaseRelocationsSection(DWORD base, PIMAGE_NT_HEADERS pNTHeader)
{
	QString str;

	DWORD dwBaseRelocRVA;
	PIMAGE_BASE_RELOCATION baseReloc;

	dwBaseRelocRVA = GetImgDirEntryRVA(pNTHeader, IMAGE_DIRECTORY_ENTRY_BASERELOC);
	if(!dwBaseRelocRVA)
		return str;

	baseReloc = (PIMAGE_BASE_RELOCATION)GetPtrFromRVA(dwBaseRelocRVA, pNTHeader, base);

	if(!baseReloc)
		return str;

	str += QString().sprintf("base relocations:\n\n");

	while(baseReloc->SizeOfBlock != 0)
	{
		unsigned i,cEntries;
		PWORD pEntry;
		char* szRelocType;
		WORD relocType;

		// Sanity check to make sure the data looks OK.
		if(0 == baseReloc->VirtualAddress)
			break;
		if(baseReloc->SizeOfBlock < sizeof(*baseReloc))
			break;

		cEntries = (baseReloc->SizeOfBlock-sizeof(*baseReloc))/sizeof(WORD);
		pEntry = MakePtr( PWORD, baseReloc, sizeof(*baseReloc) );

		str += QString().sprintf("Virtual Address: %08X  size: %08X\n",
				baseReloc->VirtualAddress, baseReloc->SizeOfBlock);

		for(i = 0; i < cEntries; i++)
		{
			// Extract the top 4 bits of the relocation entry.  Turn those 4
			// bits into an appropriate descriptive string (szRelocType)
			relocType = (*pEntry & 0xF000) >> 12;
			szRelocType = (char*)(relocType < 8 ? SzRelocTypes[relocType] : "unknown");

			str += QString().sprintf("  %08X %s",
					(*pEntry & 0x0FFF) + baseReloc->VirtualAddress,
					szRelocType);

			if ( IMAGE_REL_BASED_HIGHADJ == relocType )
			{
				pEntry++;
				cEntries--;
				str += QString().sprintf(" (%X)", *pEntry );
			}

			str += QString().sprintf("\n" );
			pEntry++; // Advance to next relocation entry
		}

		baseReloc = MakePtr(PIMAGE_BASE_RELOCATION, baseReloc, baseReloc->SizeOfBlock);
	}

	return str;
}

QWidget* TreeItemBaseRelocationsSection::getWidget() const
{
	return createTextEdit(DumpBaseRelocationsSection(base, pNTHeader));
}
