#ifndef TREEITEMFILEHEADER_H
#define TREEITEMFILEHEADER_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemFileHeader : public TreeItem
{
public:
	PIMAGE_FILE_HEADER pImageFileHeader;

	TreeItemFileHeader(TreeItem* parent, PIMAGE_FILE_HEADER pImageFileHeader) : TreeItem(parent)
	{
		this->pImageFileHeader = pImageFileHeader;
	}
	virtual QString getLabel() const
	{
		return "FileHeader";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMFILEHEADER_H
