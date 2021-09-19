#include "pedump.h"
#include <QMessageBox>

#include "items/TreeItemFileHeader.h"
#include "items/TreeItemROMOptionalHeader.h"
#include "items/TreeItemSectionTable.h"

// VARIATION on the IMAGE_FIRST_SECTION macro from WINNT.H

#define IMAGE_FIRST_ROM_SECTION(ntheader) ((PIMAGE_SECTION_HEADER)				\
	((DWORD)ntheader +															\
		FIELD_OFFSET(IMAGE_ROM_HEADERS, OptionalHeader) +						\
		((PIMAGE_ROM_HEADERS)(ntheader))->FileHeader.SizeOfOptionalHeader		\
	))

void PEDumpModel::loadROMImage(TreeItem* parentItem, PIMAGE_ROM_HEADERS pROMHeader)
{
	new TreeItemFileHeader(parentItem, &pROMHeader->FileHeader);
	new TreeItemROMOptionalHeader(parentItem, &pROMHeader->OptionalHeader);
	new TreeItemSectionTable(parentItem, 0, IMAGE_FIRST_ROM_SECTION(pROMHeader), pROMHeader->FileHeader.NumberOfSections, TRUE);

	// Dump COFF symbols out here.  Get offsets from the header
}
