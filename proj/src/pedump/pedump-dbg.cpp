#include "pedump.h"
#include <QMessageBox>

#include "items/TreeItemImageDbgHeader.h"
#include "items/TreeItemSectionTable.h"
#include "items/TreeItemDebugDirectory.h"
#include "items/TreeItemCOFFHeader.h"
#include "items/TreeItemSymbolTable.h"

void PEDumpModel::loadDbgFile(TreeItem* parentItem, PIMAGE_SEPARATE_DEBUG_HEADER pImageSepDbgHeader)
{
	// Image debug header
	new TreeItemImageDbgHeader(
		parentItem,
		pImageSepDbgHeader);

	// Section table
	new TreeItemSectionTable(
		parentItem,
		0,
		(PIMAGE_SECTION_HEADER)(pImageSepDbgHeader+1),
		pImageSepDbgHeader->NumberOfSections,
		TRUE);

	// Debug directory
	PIMAGE_DEBUG_DIRECTORY debugDir = MakePtr(PIMAGE_DEBUG_DIRECTORY,
		pImageSepDbgHeader, sizeof(IMAGE_SEPARATE_DEBUG_HEADER) +
		(pImageSepDbgHeader->NumberOfSections * sizeof(IMAGE_SECTION_HEADER))
		+ pImageSepDbgHeader->ExportedNamesSize);
	DWORD debugDirSize = pImageSepDbgHeader->DebugDirectorySize;
	DWORD debugDirBase = (DWORD)pImageSepDbgHeader;

	initDebugDirectories(debugDir, debugDirSize, debugDirBase);
	new TreeItemDebugDirectory(parentItem, debugDir, debugDirSize, debugDirBase);

	if(pCOFFHeader)
	{
		// COFF Header
		new TreeItemCOFFHeader(parentItem, pCOFFHeader);

		// Symbol table

		pCOFFSymbolTable = new COFFSymbolTable(
			MakePtr(PVOID, pCOFFHeader, pCOFFHeader->LvaToFirstSymbol),
			pCOFFHeader->NumberOfSymbols);

		new TreeItemSymbolTable(parentItem, pCOFFSymbolTable);
	}
}
