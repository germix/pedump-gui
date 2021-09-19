#ifndef TREEITEMMISCDEBUGINFO_H
#define TREEITEMMISCDEBUGINFO_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemMiscDebugInfo : public TreeItem
{
public:
	PIMAGE_DEBUG_MISC pMiscDebugInfo;

	TreeItemMiscDebugInfo(TreeItem* parent, PIMAGE_DEBUG_MISC pMiscDebugInfo) : TreeItem(parent)
	{
		this->pMiscDebugInfo = pMiscDebugInfo;
	}
	virtual QString getLabel() const
	{
		return "MiscDebugInfo";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMMISCDEBUGINFO_H
