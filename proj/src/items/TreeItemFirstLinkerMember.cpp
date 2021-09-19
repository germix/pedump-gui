#include "TreeItemFirstLinkerMember.h"

// Routine to convert from big endian to little endian
DWORD ConvertBigEndian(DWORD bigEndian)
{
	DWORD temp = 0;

	// printf( "bigEndian: %08X\n", bigEndian );

	temp |= bigEndian >> 24;
	temp |= ((bigEndian & 0x00FF0000) >> 8);
	temp |= ((bigEndian & 0x0000FF00) << 8);
	temp |= ((bigEndian & 0x000000FF) << 24);

	return temp;
}

QWidget* TreeItemFirstLinkerMember::getWidget() const
{
	QString str;
	DWORD cSymbols = *(PDWORD)p;
	PDWORD pMemberOffsets = MakePtr( PDWORD, p, 4 );
	PSTR pSymbolName;

	cSymbols = ConvertBigEndian(cSymbols);
	pSymbolName = MakePtr( PSTR, pMemberOffsets, 4 * cSymbols );

	str += QString().sprintf("First Linker Member:\n");
	str += QString().sprintf("  Symbols:         %08X\n", cSymbols);
	str += QString().sprintf("  MbrOffs   Name\n  --------  ----\n");

	for(DWORD i = 0; i < cSymbols; i++)
	{
		DWORD offset;

		offset = ConvertBigEndian( *pMemberOffsets );

		str += QString().sprintf("  %08X  %s\n", offset, pSymbolName);

		pMemberOffsets++;
		pSymbolName += strlen(pSymbolName) + 1;
	}

	return createTextEdit(str);
}
