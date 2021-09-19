#include "TreeItemCOFFHeader.h"

//
// Dump the COFF debug information header
//
QString DumpCOFFHeader(PIMAGE_COFF_SYMBOLS_HEADER pDbgInfo)
{
	QString str;

	str += QString().sprintf("COFF Debug Info Header\n");
	str += QString().sprintf("  NumberOfSymbols:      %08X\n", pDbgInfo->NumberOfSymbols);
	str += QString().sprintf("  LvaToFirstSymbol:     %08X\n", pDbgInfo->LvaToFirstSymbol);
	str += QString().sprintf("  NumberOfLinenumbers:  %08X\n", pDbgInfo->NumberOfLinenumbers);
	str += QString().sprintf("  LvaToFirstLinenumber: %08X\n", pDbgInfo->LvaToFirstLinenumber);
	str += QString().sprintf("  RvaToFirstByteOfCode: %08X\n", pDbgInfo->RvaToFirstByteOfCode);
	str += QString().sprintf("  RvaToLastByteOfCode:  %08X\n", pDbgInfo->RvaToLastByteOfCode);
	str += QString().sprintf("  RvaToFirstByteOfData: %08X\n", pDbgInfo->RvaToFirstByteOfData);
	str += QString().sprintf("  RvaToLastByteOfData:  %08X\n", pDbgInfo->RvaToLastByteOfData);

	return str;
}

QWidget* TreeItemCOFFHeader::getWidget() const
{
	return createTextEdit(DumpCOFFHeader(pDbgInfo));
}
