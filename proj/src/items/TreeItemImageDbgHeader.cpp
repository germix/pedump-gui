#include "TreeItemImageDbgHeader.h"

QString DumpImageDbgHeader(PIMAGE_SEPARATE_DEBUG_HEADER pImageSepDbgHeader)
{
	QString str;
	UINT headerFieldWidth = 30;

	str += QString().sprintf("  %-*s%04X\n",
				headerFieldWidth,
				"Flags:",
				pImageSepDbgHeader->Flags);
	str += QString().sprintf("  %-*s%04X %s\n",
				headerFieldWidth,
				"Machine:",
				pImageSepDbgHeader->Machine,
				GetMachineTypeName(pImageSepDbgHeader->Machine));
	str += QString().sprintf("  %-*s%04X\n",
				headerFieldWidth,
				"Characteristics:",
				pImageSepDbgHeader->Characteristics);
	str += QString().sprintf("  %-*s%08X -> %s",
				headerFieldWidth,
				"TimeDateStamp:",
				pImageSepDbgHeader->TimeDateStamp,
				ctime((time_t *)&pImageSepDbgHeader->TimeDateStamp) );
	str += QString().sprintf("  %-*s%08X\n",
				headerFieldWidth,
				"CheckSum:",
				pImageSepDbgHeader->CheckSum);
	str += QString().sprintf("  %-*s%08X\n",
				headerFieldWidth,
				"ImageBase:",
				pImageSepDbgHeader->ImageBase);
	str += QString().sprintf("  %-*s%08X\n",
				headerFieldWidth,
				"Size of Image:",
				pImageSepDbgHeader->SizeOfImage);
	str += QString().sprintf("  %-*s%04X\n",
				headerFieldWidth,
				"Number of Sections:",
				pImageSepDbgHeader->NumberOfSections);
	str += QString().sprintf("  %-*s%04X\n",
				headerFieldWidth,
				"ExportedNamesSize:",
				pImageSepDbgHeader->ExportedNamesSize);
	str += QString().sprintf("  %-*s%08X\n",
				headerFieldWidth,
				"DebugDirectorySize:",
				pImageSepDbgHeader->DebugDirectorySize);
	str += QString().sprintf("  %-*s%08X\n",
				headerFieldWidth,
				"SectionAlignment:",
				pImageSepDbgHeader->SectionAlignment);

	return str;
}

QWidget* TreeItemImageDbgHeader::getWidget() const
{
	return createTextEdit(DumpImageDbgHeader(pImageSepDbgHeader));
}
