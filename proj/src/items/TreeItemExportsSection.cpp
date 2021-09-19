#include "TreeItemExportsSection.h"

//
// Dump the exports table (usually the .edata section) of a PE file
//
QString DumpExportsSection(DWORD base, PIMAGE_NT_HEADERS pNTHeader)
{
	QString str;

	PIMAGE_EXPORT_DIRECTORY exportDir;
	PIMAGE_SECTION_HEADER header;
	INT delta;
	PSTR filename;
	DWORD i;
	PDWORD functions;
	PWORD ordinals;
	PSTR *name;
	DWORD exportsStartRVA, exportsEndRVA;

	exportsStartRVA = GetImgDirEntryRVA(pNTHeader,IMAGE_DIRECTORY_ENTRY_EXPORT);
	exportsEndRVA = exportsStartRVA +
					GetImgDirEntrySize(pNTHeader, IMAGE_DIRECTORY_ENTRY_EXPORT);

	// Get the IMAGE_SECTION_HEADER that contains the exports.  This is
	// usually the .edata section, but doesn't have to be.
	header = GetEnclosingSectionHeader( exportsStartRVA, pNTHeader );
	if(!header)
		return str;

	delta = (INT)(header->VirtualAddress - header->PointerToRawData);

	exportDir = MakePtr(PIMAGE_EXPORT_DIRECTORY, base,
						 exportsStartRVA - delta);

	filename = (PSTR)(exportDir->Name - delta + base);

	str += QString().sprintf("exports table:\n\n");
	str += QString().sprintf("  Name:            %s\n", filename);
	str += QString().sprintf("  Characteristics: %08X\n", exportDir->Characteristics);
	str += QString().sprintf("  TimeDateStamp:   %08X -> %s", exportDir->TimeDateStamp, ctime((time_t *)&exportDir->TimeDateStamp) );
	str += QString().sprintf("  Version:         %u.%02u\n", exportDir->MajorVersion, exportDir->MinorVersion);
	str += QString().sprintf("  Ordinal base:    %08X\n", exportDir->Base);
	str += QString().sprintf("  # of functions:  %08X\n", exportDir->NumberOfFunctions);
	str += QString().sprintf("  # of Names:      %08X\n", exportDir->NumberOfNames);

	functions = (PDWORD)((DWORD)exportDir->AddressOfFunctions - delta + base);
	ordinals = (PWORD)((DWORD)exportDir->AddressOfNameOrdinals - delta + base);
	name = (PSTR *)((DWORD)exportDir->AddressOfNames - delta + base);

	str += QString().sprintf("\n  Entry Pt  Ordn  Name\n");
	for(DWORD i = 0; i < exportDir->NumberOfFunctions; i++)
	{
		DWORD entryPointRVA = functions[i];

		if(entryPointRVA == 0)		// Skip over gaps in exported function
			continue;				// ordinals (the entrypoint is 0 for
									// these functions).

		str += QString().sprintf("  %08X  %4u", entryPointRVA, i + exportDir->Base );

		// See if this function has an associated name exported for it.
		for(DWORD j = 0; j < exportDir->NumberOfNames; j++)
		{
			if ( ordinals[j] == i )
				str += QString().sprintf("  %s", name[j] - delta + base);
		}

		// Is it a forwarder?  If so, the entry point RVA is inside the
		// .edata section, and is an RVA to the DllName.EntryPointName
		if((entryPointRVA >= exportsStartRVA) && (entryPointRVA <= exportsEndRVA))
		{
			str += QString().sprintf(" (forwarder -> %s)", entryPointRVA - delta + base );
		}

		str += QString().sprintf("\n");
	}

	return str;
}

QWidget* TreeItemExportsSection::getWidget() const
{
	return createTextEdit(DumpExportsSection(base, pNTHeader));
}
