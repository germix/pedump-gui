#include "TreeItemSectionTable.h"
#include "TreeItemSection.h"

//
// Dump the section table from a PE file or an OBJ
//
QString DumpSectionTable(PIMAGE_SECTION_HEADER section, unsigned int cSections, BOOL IsEXE)
{
	QString str;
	PCSTR pszAlign;

	if(cSections == 0xFFFF)
	{
		return str;
	}

	str += QString().sprintf("Section Table\n");

	for(unsigned int i = 1; i <= cSections; i++, section++)
	{
		str += QString().sprintf("  %02X %-8.8s  %s: %08X  VirtAddr:  %08X\n",
				i, section->Name,
				IsEXE ? "VirtSize" : "PhysAddr",
				section->Misc.VirtualSize,
				section->VirtualAddress);
		str += QString().sprintf("    raw data offs:   %08X  raw data size: %08X\n",
				section->PointerToRawData,
				section->SizeOfRawData );
		str += QString().sprintf("    relocation offs: %08X  relocations:   %08X\n",
				section->PointerToRelocations,
				section->NumberOfRelocations );
		str += QString().sprintf("    line # offs:     %08X  line #'s:      %08X\n",
				section->PointerToLinenumbers,
				section->NumberOfLinenumbers );
		str += QString().sprintf("    characteristics: %08X\n",
				section->Characteristics);

		str += QString().sprintf("    ");
		for(int j = 0; j < NUMBER_SECTION_CHARACTERISTICS; j++)
		{
			if(section->Characteristics & SectionCharacteristics[j].flag)
			{
				str += QString().sprintf("  %s", SectionCharacteristics[j].name );
			}
		}

		switch(section->Characteristics & IMAGE_SCN_ALIGN_64BYTES)
		{
			case IMAGE_SCN_ALIGN_1BYTES: pszAlign = "ALIGN_1BYTES"; break;
			case IMAGE_SCN_ALIGN_2BYTES: pszAlign = "ALIGN_2BYTES"; break;
			case IMAGE_SCN_ALIGN_4BYTES: pszAlign = "ALIGN_4BYTES"; break;
			case IMAGE_SCN_ALIGN_8BYTES: pszAlign = "ALIGN_8BYTES"; break;
			case IMAGE_SCN_ALIGN_16BYTES: pszAlign = "ALIGN_16BYTES"; break;
			case IMAGE_SCN_ALIGN_32BYTES: pszAlign = "ALIGN_32BYTES"; break;
			case IMAGE_SCN_ALIGN_64BYTES: pszAlign = "ALIGN_64BYTES"; break;
			default: pszAlign = "ALIGN_DEFAULT(16)"; break;
		}
		str += QString().sprintf("  %s", pszAlign );

		str += QString().sprintf("\n\n");
	}
	return str;
}

TreeItemSectionTable::TreeItemSectionTable(TreeItem* parent, DWORD base, PIMAGE_SECTION_HEADER section, unsigned int cSections, BOOL IsEXE) : TreeItem(parent)
{
	this->section = section;
	this->cSections = cSections;
	this->IsEXE = IsEXE;

	if(base && cSections != 0xFFFF)
	{
		for(int i = 0; i < cSections; i++)
		{
			char name[IMAGE_SIZEOF_SHORT_NAME + 1];
			PIMAGE_SECTION_HEADER sec = &section[i];

			// Make a copy of the section name so that we can ensure that
			// it's null-terminated
			memcpy(name, sec->Name, IMAGE_SIZEOF_SHORT_NAME);
			name[IMAGE_SIZEOF_SHORT_NAME] = 0;

			// Don't dump sections that don't exist in the file!
			if(sec->PointerToRawData == 0)
				continue;

			new TreeItemSection(this, base, i, name, sec);
		}
	}
}
QWidget* TreeItemSectionTable::getWidget() const
{
	return createTextEdit(DumpSectionTable(section, cSections, IsEXE));
}
