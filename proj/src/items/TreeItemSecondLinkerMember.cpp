#include "TreeItemSecondLinkerMember.h"

QWidget* TreeItemSecondLinkerMember::getWidget() const
{
	QString str;

	DWORD cArchiveMembers = *(PDWORD)p;
	PDWORD pMemberOffsets = MakePtr( PDWORD, p, 4 );
	DWORD cSymbols;
	PSTR pSymbolName;
	PWORD pIndices;

	cArchiveMembers = cArchiveMembers;

	// The number of symbols is in the DWORD right past the end of the
	// member offset array.
	cSymbols = pMemberOffsets[cArchiveMembers];

	pIndices = MakePtr( PWORD, p, 4 + cArchiveMembers * sizeof(DWORD) + 4 );

	pSymbolName = MakePtr( PSTR, pIndices, cSymbols * sizeof(WORD) );

	str += QString().sprintf("Second Linker Member:\n");

	str += QString().sprintf("  Archive Members: %08X\n", cArchiveMembers);
	str += QString().sprintf("  Symbols:         %08X\n", cSymbols);
	str += QString().sprintf("  MbrOffs   Name\n  --------  ----\n");

	for(DWORD i = 0; i < cSymbols; i++)
	{
		str += QString().sprintf("  %08X  %s\n", pMemberOffsets[pIndices[i] - 1], pSymbolName);
		pSymbolName += strlen(pSymbolName) + 1;
	}

	return createTextEdit(str);
}
