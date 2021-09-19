#ifndef TREEITEMCOFFHEADER_H
#define TREEITEMCOFFHEADER_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemCOFFHeader : public TreeItem
{
public:
	PIMAGE_COFF_SYMBOLS_HEADER pDbgInfo;

	TreeItemCOFFHeader(TreeItem* parent, PIMAGE_COFF_SYMBOLS_HEADER pDbgInfo) : TreeItem(parent)
	{
		this->pDbgInfo = pDbgInfo;
	}
	virtual QString getLabel() const
	{
		return "COFFHeader";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMCOFFHEADER_H
