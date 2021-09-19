#ifndef TREEITEMIMAGEDBGHEADER_H
#define TREEITEMIMAGEDBGHEADER_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemImageDbgHeader : public TreeItem
{
public:
	PIMAGE_SEPARATE_DEBUG_HEADER pImageSepDbgHeader;

	TreeItemImageDbgHeader(TreeItem* parent, PIMAGE_SEPARATE_DEBUG_HEADER pImageSepDbgHeader) : TreeItem(parent)
	{
		this->pImageSepDbgHeader = pImageSepDbgHeader;
	}
	virtual QString getLabel() const
	{
		return "ImageDbgHeader";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMIMAGEDBGHEADER_H
