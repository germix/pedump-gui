#include "TreeItemLineNumbers.h"

//
// Given a COFF symbol table index, look up its name.  This function assumes
// that the COFFSymbolCount and PCOFFSymbolTable variables have been already
// set up.
//
BOOL LookupSymbolName(PCOFFSymbolTable pCOFFSymbolTable, DWORD index, PSTR buffer, UINT length)
{
	if(!pCOFFSymbolTable)
		return FALSE;

	PCOFFSymbol pSymbol = pCOFFSymbolTable->GetSymbolFromIndex(index);

	if(!pSymbol)
		return FALSE;

	strncpy(buffer, pSymbol->GetName(), length );

	delete pSymbol;

	return TRUE;
}

//
// Dump a range of line numbers from the COFF debug information
//
QString DumpLineNumbers(COFFSymbolTable* pCOFFSymbolTable, PIMAGE_LINENUMBER pln, DWORD count)
{
	QString str;

	char buffer[64];

	str += QString().sprintf("Line Numbers\n");

	for(DWORD i = 0; i < count; i++)
	{
		if(pln->Linenumber == 0) // A symbol table index
		{
			buffer[0] = 0;
			LookupSymbolName(pCOFFSymbolTable, pln->Type.SymbolTableIndex, buffer, sizeof(buffer));
			str += QString().sprintf("SymIndex: %X (%s)\n",
					pln->Type.SymbolTableIndex,
					buffer);
		}
		else // A regular line number
		{
			str += QString().sprintf(" Addr: %05X  Line: %04u\n", pln->Type.VirtualAddress, pln->Linenumber);
		}
		pln++;
	}

	return str;
}

QWidget* TreeItemLineNumbers::getWidget() const
{
	return createTextEdit(DumpLineNumbers(pCOFFSymbolTable, pln, count));
}
