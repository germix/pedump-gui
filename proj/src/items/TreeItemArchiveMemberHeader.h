#ifndef TREEITEMARCHIVEMEMBERHEADER_H
#define TREEITEMARCHIVEMEMBERHEADER_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemArchiveMemberHeader : public TreeItem
{
public:
	PCSTR PszLongnames;
	PIMAGE_ARCHIVE_MEMBER_HEADER pArchHeader;
	DWORD fileOffset;

	TreeItemArchiveMemberHeader(TreeItem* parent, PCSTR PszLongnames, PIMAGE_ARCHIVE_MEMBER_HEADER pArchHeader, DWORD fileOffset) : TreeItem(parent)
	{
		this->PszLongnames = PszLongnames;
		this->pArchHeader = pArchHeader;
		this->fileOffset = fileOffset;
	}
	virtual QString getLabel() const
	{
		return "ArchiveMemberHeader";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMARCHIVEMEMBERHEADER_H
