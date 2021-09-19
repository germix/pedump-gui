#include "TreeItemRuntimeFunctions.h"

QString DumpRuntimeFunctions(COFFSymbolTable* pCOFFSymbolTable, DWORD base, PIMAGE_NT_HEADERS pNTHeader)
{
	QString str;
	DWORD rtFnRVA;

	rtFnRVA = GetImgDirEntryRVA(pNTHeader, IMAGE_DIRECTORY_ENTRY_EXCEPTION);
	if(!rtFnRVA)
		return str;

	DWORD cEntries
		= GetImgDirEntrySize(pNTHeader, IMAGE_DIRECTORY_ENTRY_EXCEPTION)
		/ sizeof( IMAGE_RUNTIME_FUNCTION_ENTRY);
	if(0 == cEntries)
		return str;

	PIMAGE_RUNTIME_FUNCTION_ENTRY pRTFn = (PIMAGE_RUNTIME_FUNCTION_ENTRY)GetPtrFromRVA(rtFnRVA, pNTHeader, base);
	if(!pRTFn)
		return str;

	str += QString().sprintf("Runtime Function Table (Exception handling)\n" );
	str += QString().sprintf("  Begin     End\n" );
	str += QString().sprintf("  --------  --------  --------\n" );

	for(unsigned i = 0; i < cEntries; i++, pRTFn++)
	{
		str += QString().sprintf("  %08X  %08X", pRTFn->BeginAddress, pRTFn->EndAddress);

		if(pCOFFSymbolTable)
		{
			PCOFFSymbol pSymbol = pCOFFSymbolTable->GetNearestSymbolFromRVA(
							pRTFn->BeginAddress - pNTHeader->OptionalHeader.ImageBase,
							TRUE);

			if(pSymbol)
				str += QString().sprintf("  %s", pSymbol->GetName());

			delete pSymbol;
		}

		str += "\n";
	}

	return str;
}

QWidget* TreeItemRuntimeFunctions::getWidget() const
{
	return createTextEdit(DumpRuntimeFunctions(pCOFFSymbolTable, base, pNTHeader));
}
