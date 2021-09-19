#include "TreeItemROMOptionalHeader.h"

QString DumpROMOptionalHeader(PIMAGE_ROM_OPTIONAL_HEADER pROMOptHdr)
{
	QString str;
	UINT width = 30;

	str += QString().sprintf("Optional Header\n");

	str += QString().sprintf("  %-*s%04X\n", width, "Magic", pROMOptHdr->Magic);
	str += QString().sprintf("  %-*s%u.%02u\n", width, "linker version", pROMOptHdr->MajorLinkerVersion, pROMOptHdr->MinorLinkerVersion);
	str += QString().sprintf("  %-*s%X\n", width, "size of code", pROMOptHdr->SizeOfCode);
	str += QString().sprintf("  %-*s%X\n", width, "size of initialized data", pROMOptHdr->SizeOfInitializedData);
	str += QString().sprintf("  %-*s%X\n", width, "size of uninitialized data", pROMOptHdr->SizeOfUninitializedData);
	str += QString().sprintf("  %-*s%X\n", width, "entrypoint RVA", pROMOptHdr->AddressOfEntryPoint);
	str += QString().sprintf("  %-*s%X\n", width, "base of code", pROMOptHdr->BaseOfCode);
	str += QString().sprintf("  %-*s%X\n", width, "base of Bss", pROMOptHdr->BaseOfBss);
	str += QString().sprintf("  %-*s%X\n", width, "GprMask", pROMOptHdr->GprMask);
	str += QString().sprintf("  %-*s%X\n", width, "CprMask[0]", pROMOptHdr->CprMask[0] );
	str += QString().sprintf("  %-*s%X\n", width, "CprMask[1]", pROMOptHdr->CprMask[1] );
	str += QString().sprintf("  %-*s%X\n", width, "CprMask[2]", pROMOptHdr->CprMask[2] );
	str += QString().sprintf("  %-*s%X\n", width, "CprMask[3]", pROMOptHdr->CprMask[3] );

	str += QString().sprintf("  %-*s%X\n", width, "GpValue", pROMOptHdr->GpValue);

	return str;
}

QWidget* TreeItemROMOptionalHeader::getWidget() const
{
	return createTextEdit(DumpROMOptionalHeader(pROMOptHdr));
}
