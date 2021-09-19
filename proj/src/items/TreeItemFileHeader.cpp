#include "TreeItemFileHeader.h"

//
// Dump the IMAGE_FILE_HEADER for a PE file or an OBJ
//
QString DumpHeader(PIMAGE_FILE_HEADER pImageFileHeader)
{
	QString str;
	UINT headerFieldWidth = 30;

	str += "File Header\n";

	str += QString().sprintf("  %-*s%04X (%s)\n",
				headerFieldWidth,
				"Machine:",
				pImageFileHeader->Machine,
				GetMachineTypeName(pImageFileHeader->Machine) );
	str += QString().sprintf("  %-*s%04X\n",
				headerFieldWidth,
				"Number of Sections:",
				pImageFileHeader->NumberOfSections);
	str += QString().sprintf("  %-*s%08X -> %s",
				headerFieldWidth,
				"TimeDateStamp:",
				pImageFileHeader->TimeDateStamp,
				ctime((time_t *)&pImageFileHeader->TimeDateStamp));
	str += QString().sprintf("  %-*s%08X\n",
				headerFieldWidth,
				"PointerToSymbolTable:",
				pImageFileHeader->PointerToSymbolTable);
	str += QString().sprintf("  %-*s%08X\n",
				headerFieldWidth,
				"NumberOfSymbols:",
				pImageFileHeader->NumberOfSymbols);
	str += QString().sprintf("  %-*s%04X\n",
				headerFieldWidth,
				"SizeOfOptionalHeader:",
				pImageFileHeader->SizeOfOptionalHeader);
	str += QString().sprintf("  %-*s%04X\n",
				headerFieldWidth,
				"Characteristics:",
				pImageFileHeader->Characteristics);

	for(UINT i = 0; i < NUMBER_IMAGE_HEADER_FLAGS; i++)
	{
		if(pImageFileHeader->Characteristics & ImageFileHeaderCharacteristics[i].flag)
		{
			str += QString().sprintf("    %s\n", ImageFileHeaderCharacteristics[i].name );
		}
	}

	return str;
}

QWidget* TreeItemFileHeader::getWidget() const
{
	return createTextEdit(DumpHeader(pImageFileHeader));
}
