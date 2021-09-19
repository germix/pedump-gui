#include "TreeItemSymbolTable.h"

//
// Dumps a COFF symbol table from an EXE or OBJ
//
QString DumpSymbolTable(PCOFFSymbolTable pSymTab)
{
	QString str;

	str += QString().sprintf("Symbol Table - %X entries  (* = auxillary symbol)\n",
			pSymTab->GetNumberOfSymbols() );

	str += QString().sprintf(
	"Indx Sectn Value    Type  Storage  Name\n"
	"---- ----- -------- ----- -------  --------\n");

	PCOFFSymbol pSymbol = pSymTab->GetNextSymbol(0);

	while(pSymbol != NULL)
	{
		char szSection[10];
		GetSectionName(pSymbol->GetSectionNumber(),szSection,sizeof(szSection));

		str += QString().sprintf("%04X %5s %08X  %s %-8s %s\n",
				pSymbol->GetIndex(),
				szSection,
				pSymbol->GetValue(),
				pSymbol->GetTypeName(),
				pSymbol->GetStorageClassName(),
				pSymbol->GetName() );

		if(pSymbol->GetNumberOfAuxSymbols())
		{
			char szAuxSymbol[1024];
			if (pSymbol->GetAuxSymbolAsString(szAuxSymbol,sizeof(szAuxSymbol)))
			{
				str += QString().sprintf("     * %s\n", szAuxSymbol );
			}
		}

		pSymbol = pSymTab->GetNextSymbol(pSymbol);
	}

	return str;
}

QWidget* TreeItemSymbolTable::getWidget() const
{
	return createTextEdit(DumpSymbolTable(pSymTab));
}
