#ifndef TREEITEMDEBUGDIRECTORY_H
#define TREEITEMDEBUGDIRECTORY_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemDebugDirectory : public TreeItem
{
public:
	PIMAGE_DEBUG_DIRECTORY debugDir;
	DWORD size;
	DWORD base;

	TreeItemDebugDirectory(TreeItem* parent, PIMAGE_DEBUG_DIRECTORY debugDir, DWORD size, DWORD base) : TreeItem(parent)
	{
		this->debugDir = debugDir;
		this->size = size;
		this->base = base;
	}
	virtual QString getLabel() const
	{
		return "DebugDirectory";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMDEBUGDIRECTORY_H
