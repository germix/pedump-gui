#include "TreeItemDebugDirectory.h"

const char* SzDebugFormats[] =
{
	"UNKNOWN/BORLAND",
	"COFF",
	"CODEVIEW",
	"FPO",
	"MISC",
	"EXCEPTION",
	"FIXUP",
	"OMAP_TO_SRC",
	"OMAP_FROM_SRC",
};

//
// Dump the debug directory array
//
QString DumpDebugDirectory(PIMAGE_DEBUG_DIRECTORY debugDir, DWORD size, DWORD base)
{
	QString str;

	DWORD cDebugFormats = size / sizeof(IMAGE_DEBUG_DIRECTORY);
	PSTR szDebugFormat;

	if(cDebugFormats == 0)
		return str;

	str += QString().sprintf(
	"Debug Formats in File\n"
	"  Type            Size     Address  FilePtr  Charactr TimeDate Version\n"
	"  --------------- -------- -------- -------- -------- -------- --------\n"
	);

	for(DWORD i = 0; i < cDebugFormats; i++)
	{
		szDebugFormat = (PSTR)((debugDir->Type <= IMAGE_DEBUG_TYPE_OMAP_FROM_SRC)
						? SzDebugFormats[debugDir->Type] : "???");

		str += QString().sprintf("  %-15s %08X %08X %08X %08X %08X %u.%02u\n",
			szDebugFormat, debugDir->SizeOfData, debugDir->AddressOfRawData,
			debugDir->PointerToRawData, debugDir->Characteristics,
			debugDir->TimeDateStamp, debugDir->MajorVersion,
			debugDir->MinorVersion);

		debugDir++;
	}

	return str;
}

QWidget* TreeItemDebugDirectory::getWidget() const
{
	return createTextEdit(DumpDebugDirectory(debugDir, size, base));
}
