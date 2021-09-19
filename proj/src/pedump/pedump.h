#ifndef PEDUMP_H
#define PEDUMP_H
#include <windows.h>
#include <stdio.h>
#include <time.h>

#define IMAGE_SIZEOF_ROM_OPTIONAL_HEADER 56

// MakePtr is a macro that allows you to easily add to values (including
// pointers) together without dealing with C's pointer arithmetic.  It
// essentially treats the last two parameters as DWORDs.  The first
// parameter is used to typecast the result to the appropriate pointer type.
#define MakePtr(cast, ptr, addValue) (cast)((DWORD)(ptr) + (DWORD)(addValue))

LPVOID GetSectionPtr(PSTR name, PIMAGE_NT_HEADERS pNTHeader, DWORD imageBase);
LPVOID GetPtrFromRVA(DWORD rva, PIMAGE_NT_HEADERS pNTHeader, DWORD imageBase);
void GetSectionName(WORD section, PSTR buffer, unsigned cbBuffer);
PIMAGE_SECTION_HEADER GetSectionHeader(PCSTR name, PIMAGE_NT_HEADERS pNTHeader);
PIMAGE_SECTION_HEADER GetEnclosingSectionHeader(DWORD rva, PIMAGE_NT_HEADERS pNTHeader);

PCSTR GetMachineTypeName(WORD wMachineType);

#define GetImgDirEntryRVA(pNTHdr, IDE) (pNTHdr->OptionalHeader.DataDirectory[IDE].VirtualAddress)

#define GetImgDirEntrySize(pNTHdr, IDE) (pNTHdr->OptionalHeader.DataDirectory[IDE].Size)

typedef struct
{
	WORD		flag;
	PCSTR		name;
} WORD_FLAG_DESCRIPTIONS;

typedef struct
{
	DWORD		flag;
	PCSTR		name;
} DWORD_FLAG_DESCRIPTIONS;

extern const WORD_FLAG_DESCRIPTIONS ImageFileHeaderCharacteristics[];
extern const int NUMBER_IMAGE_HEADER_FLAGS;

// Marked as obsolete in MSDN CD 9
// Bitfield values and names for the DllCharacteritics flags
extern const WORD_FLAG_DESCRIPTIONS DllCharacteristics[];
extern const int NUMBER_DLL_CHARACTERISTICS;

extern const char* ImageDirectoryNames[];
extern const int NUMBER_IMAGE_DIRECTORY_ENTRYS;

//
// These aren't defined in the NT 4.0 WINNT.H, so we'll define them here
//
#ifndef IMAGE_SCN_TYPE_DSECT
#define IMAGE_SCN_TYPE_DSECT                 0x00000001  // Reserved.
#endif
#ifndef IMAGE_SCN_TYPE_NOLOAD
#define IMAGE_SCN_TYPE_NOLOAD                0x00000002  // Reserved.
#endif
#ifndef IMAGE_SCN_TYPE_GROUP
#define IMAGE_SCN_TYPE_GROUP                 0x00000004  // Reserved.
#endif
#ifndef IMAGE_SCN_TYPE_COPY
#define IMAGE_SCN_TYPE_COPY                  0x00000010  // Reserved.
#endif
#ifndef IMAGE_SCN_TYPE_OVER
#define IMAGE_SCN_TYPE_OVER                  0x00000400  // Reserved.
#endif
#ifndef IMAGE_SCN_MEM_PROTECTED
#define IMAGE_SCN_MEM_PROTECTED              0x00004000
#endif
#ifndef IMAGE_SCN_MEM_SYSHEAP
#define IMAGE_SCN_MEM_SYSHEAP                0x00010000
#endif

// Bitfield values and names for the IMAGE_SECTION_HEADER flags
extern const DWORD_FLAG_DESCRIPTIONS SectionCharacteristics[];
extern const int NUMBER_SECTION_CHARACTERISTICS;

typedef struct _PDB_INFO
{
	CHAR		Signature[4];		// "NBxx"
	ULONG		Offset;				// always zero
	ULONG		sig;
	ULONG		age;
	CHAR		PdbName[_MAX_PATH];
} PDB_INFO, *PPDB_INFO;

#include "pedump/COFFSymbolTable.h"

#include <QString>
#include <QVector>
#include <QIcon>
#include <QAbstractItemModel>

#include "items/TreeItem.h"

class PEDumpModel : public QAbstractItemModel
{
	Q_OBJECT
	friend class TreeItem;

	TreeItem*					root;

	HANDLE						hFile;
	HANDLE						hFileMapping;
	LPVOID						lpFileBase;

	PDWORD						pCVHeader;
	PIMAGE_DEBUG_MISC			pMiscDebugInfo;
	PIMAGE_COFF_SYMBOLS_HEADER	pCOFFHeader;
	COFFSymbolTable*			pCOFFSymbolTable;
public:
	explicit PEDumpModel(QObject* parent = nullptr);
	~PEDumpModel();
public:
	bool open(const QString& fileName);
	void clear();
private:
	void loadExeFile(TreeItem* parentItem, PIMAGE_DOS_HEADER dosHeader);
	void loadObjFile(TreeItem* parentItem, PIMAGE_FILE_HEADER pImageFileHeader);
	void loadLibFile(TreeItem* parentItem, LPVOID lpFileBase);
	void loadDbgFile(TreeItem* parentItem, PIMAGE_SEPARATE_DEBUG_HEADER pImageSepDbgHeader);
	void loadROMImage(TreeItem* parentItem, PIMAGE_ROM_HEADERS pROMHeader);

	void nullify();
	void internalClear();

	void initDebugDirectories(PIMAGE_DEBUG_DIRECTORY debugDir, DWORD size, DWORD base);
	void loadExeDebugDirectory(TreeItem* parentItem, DWORD base, PIMAGE_NT_HEADERS pNTHeader);
public:
	TreeItem* toItem(const QModelIndex& index) const;
	QModelIndex toIndex(TreeItem* item, int column) const;
public:
	// Header:
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// Basic functionality:
	QModelIndex index(int row, int column,
					  const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& child) const override;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	bool hasChildren(const QModelIndex& parent) const override;
signals:
};

#endif // PEDUMP_H
