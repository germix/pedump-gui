#include "pedump.h"

// Bitfield values and names for the IMAGE_FILE_HEADER flags
const WORD_FLAG_DESCRIPTIONS ImageFileHeaderCharacteristics[] =
{
	{ IMAGE_FILE_RELOCS_STRIPPED, "RELOCS_STRIPPED" },
	{ IMAGE_FILE_EXECUTABLE_IMAGE, "EXECUTABLE_IMAGE" },
	{ IMAGE_FILE_LINE_NUMS_STRIPPED, "LINE_NUMS_STRIPPED" },
	{ IMAGE_FILE_LOCAL_SYMS_STRIPPED, "LOCAL_SYMS_STRIPPED" },
	{ IMAGE_FILE_AGGRESIVE_WS_TRIM, "AGGRESIVE_WS_TRIM" },
//	{ IMAGE_FILE_MINIMAL_OBJECT, "MINIMAL_OBJECT" }, // Removed in NT 3.5
//	{ IMAGE_FILE_UPDATE_OBJECT, "UPDATE_OBJECT" }, // Removed in NT 3.5
//	{ IMAGE_FILE_16BIT_MACHINE, "16BIT_MACHINE" }, // Removed in NT 3.5
	{ IMAGE_FILE_BYTES_REVERSED_LO, "BYTES_REVERSED_LO" },
	{ IMAGE_FILE_32BIT_MACHINE, "32BIT_MACHINE" },
	{ IMAGE_FILE_DEBUG_STRIPPED, "DEBUG_STRIPPED" },
//	{ IMAGE_FILE_PATCH, "PATCH" },
	{ IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP, "REMOVABLE_RUN_FROM_SWAP" },
	{ IMAGE_FILE_NET_RUN_FROM_SWAP, "NET_RUN_FROM_SWAP" },
	{ IMAGE_FILE_SYSTEM, "SYSTEM" },
	{ IMAGE_FILE_DLL, "DLL" },
	{ IMAGE_FILE_UP_SYSTEM_ONLY, "UP_SYSTEM_ONLY" },
	{ IMAGE_FILE_BYTES_REVERSED_HI, "BYTES_REVERSED_HI" },
};

const int NUMBER_IMAGE_HEADER_FLAGS = (sizeof(ImageFileHeaderCharacteristics) / sizeof(WORD_FLAG_DESCRIPTIONS));

//--------------------------------------------------------------------------------------------------
// Header related stuff
//--------------------------------------------------------------------------------------------------

#ifndef	IMAGE_DLLCHARACTERISTICS_WDM_DRIVER
#define IMAGE_DLLCHARACTERISTICS_WDM_DRIVER  0x2000 // Driver uses WDM model
#endif

// Marked as obsolete in MSDN CD 9
// Bitfield values and names for the DllCharacteritics flags
const WORD_FLAG_DESCRIPTIONS DllCharacteristics[] =
{
	{ IMAGE_DLLCHARACTERISTICS_WDM_DRIVER, "WDM_DRIVER" },
};
const int NUMBER_DLL_CHARACTERISTICS = (sizeof(DllCharacteristics) / sizeof(WORD_FLAG_DESCRIPTIONS));

#if 0

// Marked as obsolete in MSDN CD 9
// Bitfield values and names for the LoaderFlags flags
DWORD_FLAG_DESCRIPTIONS LoaderFlags[] =
{
	{ IMAGE_LOADER_FLAGS_BREAK_ON_LOAD, "BREAK_ON_LOAD" },
	{ IMAGE_LOADER_FLAGS_DEBUG_ON_LOAD, "DEBUG_ON_LOAD" },
};
#define NUMBER_LOADER_FLAGS (sizeof(LoaderFlags) / sizeof(DWORD_FLAG_DESCRIPTIONS))

#endif

// Names of the data directory elements that are defined
const char* ImageDirectoryNames[] =
{
	"EXPORT",
	"IMPORT",
	"RESOURCE",
	"EXCEPTION",
	"SECURITY",
	"BASERELOC",
	"DEBUG",
	"COPYRIGHT",
	"GLOBALPTR",
	"TLS",
	"LOAD_CONFIG",

	// These two entries added for NT 3.51
	"BOUND_IMPORT",
	"IAT",

	// This entry added in NT 5
	"DELAY_IMPORT",
};

const int NUMBER_IMAGE_DIRECTORY_ENTRYS = (sizeof(ImageDirectoryNames)/sizeof(char *));

PCSTR GetMachineTypeName(WORD wMachineType)
{
	switch( wMachineType )
	{
		case IMAGE_FILE_MACHINE_I386:		return "i386";
//		case IMAGE_FILE_MACHINE_I860:		return "i860";
		case IMAGE_FILE_MACHINE_R3000:		return "R3000";
		case 160:							return "R3000 big endian";
		case IMAGE_FILE_MACHINE_R4000:		return "R4000";
		case IMAGE_FILE_MACHINE_R10000:		return "R10000";
		case IMAGE_FILE_MACHINE_ALPHA:		return "Alpha";
		case IMAGE_FILE_MACHINE_POWERPC:	return "PowerPC";
		default:							return "unknown";
	}
}

//--------------------------------------------------------------------------------------------------
// Section related stuff
//--------------------------------------------------------------------------------------------------

// Bitfield values and names for the IMAGE_SECTION_HEADER flags
const DWORD_FLAG_DESCRIPTIONS SectionCharacteristics[] =
{
	{ IMAGE_SCN_TYPE_DSECT, "DSECT" },
	{ IMAGE_SCN_TYPE_NOLOAD, "NOLOAD" },
	{ IMAGE_SCN_TYPE_GROUP, "GROUP" },
	{ IMAGE_SCN_TYPE_NO_PAD, "NO_PAD" },
	{ IMAGE_SCN_TYPE_COPY, "COPY" },
	{ IMAGE_SCN_CNT_CODE, "CODE" },
	{ IMAGE_SCN_CNT_INITIALIZED_DATA, "INITIALIZED_DATA" },
	{ IMAGE_SCN_CNT_UNINITIALIZED_DATA, "UNINITIALIZED_DATA" },
	{ IMAGE_SCN_LNK_OTHER, "OTHER" },
	{ IMAGE_SCN_LNK_INFO, "INFO" },
	{ IMAGE_SCN_TYPE_OVER, "OVER" },
	{ IMAGE_SCN_LNK_REMOVE, "REMOVE" },
	{ IMAGE_SCN_LNK_COMDAT, "COMDAT" },
	{ IMAGE_SCN_MEM_PROTECTED, "PROTECTED" },
	{ IMAGE_SCN_MEM_FARDATA, "FARDATA" },
	{ IMAGE_SCN_MEM_SYSHEAP, "SYSHEAP" },
	{ IMAGE_SCN_MEM_PURGEABLE, "PURGEABLE" },
	{ IMAGE_SCN_MEM_LOCKED, "LOCKED" },
	{ IMAGE_SCN_MEM_PRELOAD, "PRELOAD" },
	{ IMAGE_SCN_LNK_NRELOC_OVFL, "NRELOC_OVFL" },
	{ IMAGE_SCN_MEM_DISCARDABLE, "DISCARDABLE" },
	{ IMAGE_SCN_MEM_NOT_CACHED, "NOT_CACHED" },
	{ IMAGE_SCN_MEM_NOT_PAGED, "NOT_PAGED" },
	{ IMAGE_SCN_MEM_SHARED, "SHARED" },
	{ IMAGE_SCN_MEM_EXECUTE, "EXECUTE" },
	{ IMAGE_SCN_MEM_READ, "READ" },
	{ IMAGE_SCN_MEM_WRITE, "WRITE" },
};
const int NUMBER_SECTION_CHARACTERISTICS = (sizeof(SectionCharacteristics) / sizeof(DWORD_FLAG_DESCRIPTIONS));

LPVOID GetPtrFromRVA(DWORD rva, PIMAGE_NT_HEADERS pNTHeader, DWORD imageBase)
{
	PIMAGE_SECTION_HEADER pSectionHdr;
	INT delta;

	pSectionHdr = GetEnclosingSectionHeader( rva, pNTHeader );
	if(!pSectionHdr)
		return 0;

	delta = (INT)(pSectionHdr->VirtualAddress-pSectionHdr->PointerToRawData);
	return (PVOID)(imageBase + rva - delta);
}

//
// Given a section name, look it up in the section table and return a
// pointer to the start of its raw data area.
//
LPVOID GetSectionPtr(PSTR name, PIMAGE_NT_HEADERS pNTHeader, DWORD imageBase)
{
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNTHeader);

	for(unsigned int i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++, section++)
	{
		if(strncmp((char*)section->Name, name, IMAGE_SIZEOF_SHORT_NAME) == 0)
		{
			return (LPVOID)(section->PointerToRawData + imageBase);
		}
	}

	return 0;
}

//
// Used by the DumpSymbolTable() routine.  It purpose is to filter out
// the non-normal section numbers and give them meaningful names.
//
void GetSectionName(WORD section, PSTR buffer, unsigned cbBuffer)
{
	char tempbuffer[10];

	switch((SHORT)section)
	{
		case IMAGE_SYM_UNDEFINED:	strcpy(tempbuffer, "UNDEF");		break;
		case IMAGE_SYM_ABSOLUTE:	strcpy(tempbuffer, "ABS");			break;
		case IMAGE_SYM_DEBUG:		strcpy(tempbuffer, "DEBUG");		break;
		default:					sprintf(tempbuffer, "%X", section);	break;
	}

	strncpy(buffer, tempbuffer, cbBuffer-1);
}

//
// Given a section name, look it up in the section table and return a
// pointer to its IMAGE_SECTION_HEADER
//
PIMAGE_SECTION_HEADER GetSectionHeader(PCSTR name, PIMAGE_NT_HEADERS pNTHeader)
{
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNTHeader);

	for(unsigned int i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++, section++)
	{
		if(0 == strncmp((char*)section->Name,name,IMAGE_SIZEOF_SHORT_NAME))
		{
			return section;
		}
	}

	return 0;
}

//
// Given an RVA, look up the section header that encloses it and return a
// pointer to its IMAGE_SECTION_HEADER
//
PIMAGE_SECTION_HEADER GetEnclosingSectionHeader(DWORD rva, PIMAGE_NT_HEADERS pNTHeader)
{
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNTHeader);

	for(unsigned int i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++, section++)
	{
		// Is the RVA within this section?
		if((rva >= section->VirtualAddress) && (rva < (section->VirtualAddress + section->Misc.VirtualSize)))
		{
			return section;
		}
	}

	return 0;
}
