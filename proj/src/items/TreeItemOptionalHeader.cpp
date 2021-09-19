#include "TreeItemOptionalHeader.h"

//
// Dump the IMAGE_OPTIONAL_HEADER from a PE file
//
QString DumpOptionalHeader(PIMAGE_OPTIONAL_HEADER optionalHeader)
{
	QString str;
	UINT width = 30;
	const char* s;
	UINT i;

	str += QString().sprintf("Optional Header\n");

	str += QString().sprintf("  %-*s%04X\n", width, "Magic", optionalHeader->Magic);
	str += QString().sprintf("  %-*s%u.%02u\n", width, "linker version", optionalHeader->MajorLinkerVersion, optionalHeader->MinorLinkerVersion);
	str += QString().sprintf("  %-*s%X\n", width, "size of code", optionalHeader->SizeOfCode);
	str += QString().sprintf("  %-*s%X\n", width, "size of initialized data", optionalHeader->SizeOfInitializedData);
	str += QString().sprintf("  %-*s%X\n", width, "size of uninitialized data", optionalHeader->SizeOfUninitializedData);
	str += QString().sprintf("  %-*s%X\n", width, "entrypoint RVA", optionalHeader->AddressOfEntryPoint);
	str += QString().sprintf("  %-*s%X\n", width, "base of code", optionalHeader->BaseOfCode);
#ifndef _WIN64
	str += QString().sprintf("  %-*s%X\n", width, "base of data", optionalHeader->BaseOfData);
#endif
	str += QString().sprintf("  %-*s%X\n", width, "image base", optionalHeader->ImageBase);

	str += QString().sprintf("  %-*s%X\n", width, "section align", optionalHeader->SectionAlignment);
	str += QString().sprintf("  %-*s%X\n", width, "file align", optionalHeader->FileAlignment);
	str += QString().sprintf("  %-*s%u.%02u\n", width, "required OS version", optionalHeader->MajorOperatingSystemVersion, optionalHeader->MinorOperatingSystemVersion);
	str += QString().sprintf("  %-*s%u.%02u\n", width, "image version", optionalHeader->MajorImageVersion, optionalHeader->MinorImageVersion);
	str += QString().sprintf("  %-*s%u.%02u\n", width, "subsystem version", optionalHeader->MajorSubsystemVersion, optionalHeader->MinorSubsystemVersion);
	str += QString().sprintf("  %-*s%X\n", width, "Win32 Version", optionalHeader->Win32VersionValue);
	str += QString().sprintf("  %-*s%X\n", width, "size of image", optionalHeader->SizeOfImage);
	str += QString().sprintf("  %-*s%X\n", width, "size of headers", optionalHeader->SizeOfHeaders);
	str += QString().sprintf("  %-*s%X\n", width, "checksum", optionalHeader->CheckSum);
	switch( optionalHeader->Subsystem )
	{
		case IMAGE_SUBSYSTEM_NATIVE: s = "Native"; break;
		case IMAGE_SUBSYSTEM_WINDOWS_GUI: s = "Windows GUI"; break;
		case IMAGE_SUBSYSTEM_WINDOWS_CUI: s = "Windows character"; break;
		case IMAGE_SUBSYSTEM_OS2_CUI: s = "OS/2 character"; break;
		case IMAGE_SUBSYSTEM_POSIX_CUI: s = "Posix character"; break;
		default: s = "unknown";
	}
	str += QString().sprintf("  %-*s%04X (%s)\n", width, "Subsystem", optionalHeader->Subsystem, s);

	// Marked as obsolete in MSDN CD 9
	str += QString().sprintf("  %-*s%04X\n", width, "DLL flags", optionalHeader->DllCharacteristics);
	for ( i=0; i < NUMBER_DLL_CHARACTERISTICS; i++ )
	{
		if ( optionalHeader->DllCharacteristics & DllCharacteristics[i].flag )
		{
			str += QString().sprintf("  %-*s%s", width, " ", DllCharacteristics[i].name );
		}
	}
	if ( optionalHeader->DllCharacteristics )
		str += QString().sprintf("\n");

	str += QString().sprintf("  %-*s%X\n", width, "stack reserve size", optionalHeader->SizeOfStackReserve);
	str += QString().sprintf("  %-*s%X\n", width, "stack commit size", optionalHeader->SizeOfStackCommit);
	str += QString().sprintf("  %-*s%X\n", width, "heap reserve size", optionalHeader->SizeOfHeapReserve);
	str += QString().sprintf("  %-*s%X\n", width, "heap commit size", optionalHeader->SizeOfHeapCommit);

#if 0
	// Marked as obsolete in MSDN CD 9
	str += QString().sprintf("  %-*s%08X\n", width, "loader flags", optionalHeader->LoaderFlags);

	for(i = 0; i < NUMBER_LOADER_FLAGS; i++)
	{
		if(optionalHeader->LoaderFlags & LoaderFlags[i].flag)
		{
			str += QString().sprintf("  %s", LoaderFlags[i].name);
		}
	}
	if(optionalHeader->LoaderFlags)
		str += QString().sprintf("\n");
#endif

	str += QString().sprintf("  %-*s%X\n", width, "RVAs & sizes", optionalHeader->NumberOfRvaAndSizes);

	str += QString().sprintf("\nData Directory\n");
	for(i = 0; i < optionalHeader->NumberOfRvaAndSizes; i++)
	{
		str += QString().sprintf("  %-12s rva: %08X  size: %08X\n",
			(i >= NUMBER_IMAGE_DIRECTORY_ENTRYS) ? "unused" : ImageDirectoryNames[i],
			optionalHeader->DataDirectory[i].VirtualAddress,
			optionalHeader->DataDirectory[i].Size);
	}

	return str;
}

QWidget* TreeItemOptionalHeader::getWidget() const
{
	return createTextEdit(DumpOptionalHeader(optionalHeader));
}
