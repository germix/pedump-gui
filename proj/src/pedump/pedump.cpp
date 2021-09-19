#include "pedump.h"
#include <windows.h>
#include <stdio.h>
#include <QMessageBox>

#include "items/TreeItemFileHeader.h"
#include "items/TreeItemCOFFHeader.h"
#include "items/TreeItemLineNumbers.h"
#include "items/TreeItemOptionalHeader.h"
#include "items/TreeItemSymbolTable.h"
#include "items/TreeItemSectionTable.h"
#include "items/TreeItemImportsSection.h"
#include "items/TreeItemBoundImportDescriptors.h"
#include "items/TreeItemResourceSection.h"
#include "items/TreeItemRuntimeFunctions.h"
#include "items/TreeItemExportsSection.h"
#include "items/TreeItemCVDebugInfo.h"
#include "items/TreeItemDebugDirectory.h"
#include "items/TreeItemBaseRelocationsSection.h"
#include "items/TreeItemMiscDebugInfo.h"

#define MAX_COLUMNS 1

PEDumpModel::PEDumpModel(QObject *parent)
	: QAbstractItemModel(parent)
{
	root = nullptr;

	nullify();
}

PEDumpModel::~PEDumpModel()
{
	internalClear();
}

bool PEDumpModel::open(const QString& fileName)
{
	beginResetModel();
	internalClear();
	root = new TreeItem(nullptr);

	PIMAGE_DOS_HEADER dosHeader;

	hFile = CreateFileW((LPCWSTR)fileName.constData(),
						GENERIC_READ,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						0);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		QMessageBox::information(nullptr, tr("Error"), tr("Couldn't open file with CreateFile()"), QMessageBox::Ok);
		endResetModel();
		return false;
	}

	hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if(hFileMapping == 0)
	{
		CloseHandle(hFile);
		QMessageBox::information(nullptr, tr("Error"), tr("Couldn't open file mapping with CreateFileMapping()"), QMessageBox::Ok);
		endResetModel();
		return false;
	}

	lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
	if(lpFileBase == 0)
	{
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
		QMessageBox::information(nullptr, tr("Error"), tr("Couldn't map view of file with MapViewOfFile()"), QMessageBox::Ok);
		endResetModel();
		return false;
	}

	dosHeader = (PIMAGE_DOS_HEADER)lpFileBase;
	PIMAGE_FILE_HEADER pImgFileHdr = (PIMAGE_FILE_HEADER)lpFileBase;

	if(dosHeader->e_magic == IMAGE_DOS_SIGNATURE )
	{
		loadExeFile(root, dosHeader);
	}
	else if(dosHeader->e_magic == IMAGE_SEPARATE_DEBUG_SIGNATURE )
	{
		loadDbgFile(root, (PIMAGE_SEPARATE_DEBUG_HEADER)lpFileBase);
	}
	else if((pImgFileHdr->Machine == IMAGE_FILE_MACHINE_I386)	// FIX THIS!!!
			||(pImgFileHdr->Machine == IMAGE_FILE_MACHINE_ALPHA))
	{
		if(0 == pImgFileHdr->SizeOfOptionalHeader) // 0 optional header means it's an OBJ
		{
			loadObjFile(root, pImgFileHdr);
		}
		else if(pImgFileHdr->SizeOfOptionalHeader == IMAGE_SIZEOF_ROM_OPTIONAL_HEADER)
		{
			loadROMImage(root, (PIMAGE_ROM_HEADERS)pImgFileHdr);
		}
	}
	else if(0 == strncmp((char*)lpFileBase, IMAGE_ARCHIVE_START, IMAGE_ARCHIVE_START_SIZE))
	{
		loadLibFile(root, lpFileBase);
	}
	else
	{
		QMessageBox::information(nullptr, tr("Error"), tr("Unrecognized file format"), QMessageBox::Ok);
	}

	endResetModel();

	return true;
}

void PEDumpModel::clear()
{
	beginResetModel();
	internalClear();
	endResetModel();
}

void PEDumpModel::nullify()
{
	root = nullptr;

	lpFileBase = nullptr;
	hFileMapping = nullptr;
	hFile = nullptr;

	pCVHeader = nullptr;
	pMiscDebugInfo = nullptr;
	pCOFFHeader = nullptr;
	pCOFFSymbolTable = nullptr;
}

void PEDumpModel::internalClear()
{
	if(pCOFFSymbolTable)
		delete pCOFFSymbolTable;

	if(lpFileBase)
	{
		UnmapViewOfFile(lpFileBase);
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
	}

	if(root)
		delete root;

	nullify();
}

void PEDumpModel::initDebugDirectories(PIMAGE_DEBUG_DIRECTORY debugDir, DWORD size, DWORD base)
{
	DWORD cDebugFormats = size / sizeof(IMAGE_DEBUG_DIRECTORY);

	if(cDebugFormats > 0)
	{
		for(DWORD i = 0; i < cDebugFormats; i++)
		{
			switch(debugDir->Type)
			{
				case IMAGE_DEBUG_TYPE_COFF:
					pCOFFHeader = (PIMAGE_COFF_SYMBOLS_HEADER)(base+ debugDir->PointerToRawData);
					break;
				case IMAGE_DEBUG_TYPE_MISC:
					pMiscDebugInfo = (PIMAGE_DEBUG_MISC)(base + debugDir->PointerToRawData);
					break;
				case IMAGE_DEBUG_TYPE_CODEVIEW:
					pCVHeader = (PDWORD)(base + debugDir->PointerToRawData);
					break;
			}
			debugDir++;
		}
	}
}

void PEDumpModel::loadExeDebugDirectory(TreeItem* parentItem, DWORD base, PIMAGE_NT_HEADERS pNTHeader)
{
	PIMAGE_DEBUG_DIRECTORY debugDir;
	PIMAGE_SECTION_HEADER header;
	DWORD va_debug_dir;
	DWORD size;

	va_debug_dir = GetImgDirEntryRVA(pNTHeader, IMAGE_DIRECTORY_ENTRY_DEBUG);

	if(va_debug_dir == 0)
		return;

	// If we found a .debug section, and the debug directory is at the
	// beginning of this section, it looks like a Borland file
	header = GetSectionHeader(".debug", pNTHeader);
	if(header && (header->VirtualAddress == va_debug_dir))
	{
		debugDir = (PIMAGE_DEBUG_DIRECTORY)(header->PointerToRawData+base);
		size = GetImgDirEntrySize(pNTHeader, IMAGE_DIRECTORY_ENTRY_DEBUG) * sizeof(IMAGE_DEBUG_DIRECTORY);
	}
	else // Look for the debug directory
	{
		header = GetEnclosingSectionHeader( va_debug_dir, pNTHeader );
		if(!header)
			return;

		size = GetImgDirEntrySize(pNTHeader, IMAGE_DIRECTORY_ENTRY_DEBUG);

		debugDir = MakePtr(PIMAGE_DEBUG_DIRECTORY, base, header->PointerToRawData + (va_debug_dir - header->VirtualAddress));
	}

	initDebugDirectories(debugDir, size, base);
	new TreeItemDebugDirectory(parentItem, debugDir, size, base);
}

TreeItem* PEDumpModel::toItem(const QModelIndex& index) const
{
	if(!index.isValid())
		return 0;
	return static_cast<TreeItem*>(index.internalPointer());
}

QModelIndex PEDumpModel::toIndex(TreeItem* item, int column) const
{
	if(!item || (item == root))
		return QModelIndex();
	TreeItem* parent = item->parent;
	if(!parent)
		parent = root;
	int row = parent->children.lastIndexOf(item);
	if(row != -1)
		return createIndex(row, column, item);
	return QModelIndex();
}

QVariant PEDumpModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(section);
	Q_UNUSED(orientation);
	Q_UNUSED(role);
	return QVariant();
}

QModelIndex PEDumpModel::index(int row, int column, const QModelIndex& parent) const
{
	if(row < 0 || column < 0 || column >= MAX_COLUMNS)
		return QModelIndex();
	TreeItem* parentItem = parent.isValid() ? toItem(parent) : root;
	if(parentItem && row < parentItem->children.count())
	{
		TreeItem* item = parentItem->children.at(row);
		if(item)
			return createIndex(row, column, item);
		return QModelIndex();
	}
	return QModelIndex();
}

QModelIndex PEDumpModel::parent(const QModelIndex& child) const
{
	if(!child.isValid())
		return QModelIndex();
	TreeItem* item = static_cast<TreeItem*>(child.internalPointer());
	if(!item || item == root)
		return QModelIndex();
	return toIndex(item->parent, 0);
}

int PEDumpModel::rowCount(const QModelIndex& parent) const
{
	if(!root)
		return 0;
	if(!parent.isValid())
		return root->children.count();
	TreeItem* parentItem = toItem(parent);
	if(parentItem)
		return parentItem->children.count();
	return 0;
}

int PEDumpModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return MAX_COLUMNS;
}

QVariant PEDumpModel::data(const QModelIndex& index, int role) const
{
	if(index.isValid())
	{
		TreeItem* item = toItem(index);

		if(role == Qt::DisplayRole)
		{
			return item->getLabel();
		}
		else if(role == Qt::DecorationRole)
		{
			return item->getIcon();
		}
	}
	return QVariant();
}

bool PEDumpModel::hasChildren(const QModelIndex& parent) const
{
	if(!root)
		return 0;
	if(!parent.isValid())
		return (root->children.count() > 0);
	TreeItem* item = toItem(parent);
	if(!item)
		return false;
	return (item->children.count() > 0);
}
