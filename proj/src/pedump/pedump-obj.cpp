#include "pedump.h"
#include <QMessageBox>

#include "items/TreeItemFileHeader.h"
#include "items/TreeItemSymbolTable.h"
#include "items/TreeItemSectionTable.h"
#include "items/TreeItemRelocations.h"
#include "items/TreeItemLineNumbers.h"

void PEDumpModel::loadObjFile(TreeItem* parentItem, PIMAGE_FILE_HEADER pImageFileHeader)
{
	PIMAGE_SECTION_HEADER pSections;

	// File header
	new TreeItemFileHeader(parentItem, pImageFileHeader);

	// Section table
	pSections = MakePtr(
				PIMAGE_SECTION_HEADER, (pImageFileHeader+1),
				pImageFileHeader->SizeOfOptionalHeader);
	new TreeItemSectionTable(
				parentItem,
				(DWORD)pImageFileHeader,
				pSections,
				pImageFileHeader->NumberOfSections,
				FALSE);

	// Relocations
	new TreeItemRelocations(parentItem, pImageFileHeader, pSections);

	if(pImageFileHeader->PointerToSymbolTable)
	{
		// Symbol table

		pCOFFSymbolTable = new COFFSymbolTable(
			MakePtr(PVOID, pImageFileHeader, pImageFileHeader->PointerToSymbolTable),
			pImageFileHeader->NumberOfSymbols);

		new TreeItemSymbolTable(parentItem, pCOFFSymbolTable);
	}

	// Walk through the section table...
	for(unsigned int i = 0; i < pImageFileHeader->NumberOfSections; i++)
	{
		// if there's any line numbers for this section, dump'em
		if(pSections->NumberOfLinenumbers)
		{
			new TreeItemLineNumbers(
				parentItem,
				pCOFFSymbolTable,
				MakePtr(PIMAGE_LINENUMBER, pImageFileHeader, pSections->PointerToLinenumbers),
				pSections->NumberOfLinenumbers);
		}
		pSections++;
	}
}
