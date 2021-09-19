#include "pedump.h"
#include <QMessageBox>

#include "items/TreeItemFileHeader.h"
#include "items/TreeItemOptionalHeader.h"
#include "items/TreeItemSectionTable.h"
#include "items/TreeItemResourceSection.h"
#include "items/TreeItemImportsSection.h"
#include "items/TreeItemBoundImportDescriptors.h"
#include "items/TreeItemExportsSection.h"
#include "items/TreeItemRuntimeFunctions.h"
#include "items/TreeItemBaseRelocationsSection.h"
#include "items/TreeItemMiscDebugInfo.h"
#include "items/TreeItemCVDebugInfo.h"
#include "items/TreeItemCOFFHeader.h"
#include "items/TreeItemLineNumbers.h"
#include "items/TreeItemSymbolTable.h"

void PEDumpModel::loadExeFile(TreeItem* parentItem, PIMAGE_DOS_HEADER dosHeader)
{
	PIMAGE_NT_HEADERS pNTHeader;
	DWORD base = (DWORD)dosHeader;

	pNTHeader = MakePtr(PIMAGE_NT_HEADERS, dosHeader, dosHeader->e_lfanew);

	if(pNTHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		QMessageBox::information(nullptr, tr("Error"), "Not a Portable Executable (PE) EXE", QMessageBox::Ok);
		return;
	}

	// File header
	new TreeItemFileHeader(parentItem, (PIMAGE_FILE_HEADER)&pNTHeader->FileHeader);

	// Optional header
	new TreeItemOptionalHeader(parentItem, (PIMAGE_OPTIONAL_HEADER)&pNTHeader->OptionalHeader);

	// Section table
	new TreeItemSectionTable(parentItem, base, IMAGE_FIRST_SECTION(pNTHeader), pNTHeader->FileHeader.NumberOfSections, TRUE);

	// Exe debug directory
	loadExeDebugDirectory(parentItem, base, pNTHeader);

	if(pNTHeader->FileHeader.PointerToSymbolTable == 0)
		pCOFFHeader = 0; // Doesn't really exist!

	// Resource section
	new TreeItemResourceSection(parentItem, base, pNTHeader);

	// Imports section
	new TreeItemImportsSection(parentItem, base, pNTHeader);

	// Bound import descriptors
	if(GetImgDirEntryRVA(pNTHeader, IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT))
	{
		new TreeItemBoundImportDescriptors(parentItem, base, pNTHeader);
	}

	// Exports section
	new TreeItemExportsSection(parentItem, base, pNTHeader);

	//=========================================================================
	//
	// If we have COFF symbols, create a symbol table now
	//
	//=========================================================================

	if(pCOFFHeader) // Did we see a COFF symbols header while looking through the debug directory?
	{
		pCOFFSymbolTable = new COFFSymbolTable(
				(PVOID)(base+ pNTHeader->FileHeader.PointerToSymbolTable),
				pNTHeader->FileHeader.NumberOfSymbols );
	}

	// RuntimeFunctions
	new TreeItemRuntimeFunctions(parentItem, pCOFFSymbolTable, base, pNTHeader);

	// BaseRelocationsSection
	new TreeItemBaseRelocationsSection(parentItem, base, pNTHeader);

	if(pMiscDebugInfo)
	{
		new TreeItemMiscDebugInfo(parentItem, pMiscDebugInfo);
	}

	if(pCVHeader)
	{
		new TreeItemCVDebugInfo(parentItem, pCVHeader);
	}

	if(pCOFFHeader)
	{
		new TreeItemCOFFHeader(parentItem, pCOFFHeader);
	}

	if(pCOFFHeader)
	{
		new TreeItemLineNumbers(
			parentItem,
			pCOFFSymbolTable,
			MakePtr(PIMAGE_LINENUMBER, pCOFFHeader, pCOFFHeader->LvaToFirstLinenumber),
			pCOFFHeader->NumberOfLinenumbers);
	}

	if(pNTHeader->FileHeader.NumberOfSymbols && pNTHeader->FileHeader.PointerToSymbolTable && pCOFFSymbolTable)
	{
		new TreeItemSymbolTable(parentItem, pCOFFSymbolTable);
	}
}
