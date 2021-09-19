#ifndef TREEITEMCVDEBUGINFO_H
#define TREEITEMCVDEBUGINFO_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemCVDebugInfo : public TreeItem
{
public:
	PDWORD pCVHeader;

	TreeItemCVDebugInfo(TreeItem* parent, PDWORD pCVHeader) : TreeItem(parent)
	{
		this->pCVHeader = pCVHeader;
	}
	virtual QString getLabel() const
	{
		return "CVDebugInfo";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMCVDEBUGINFO_H
