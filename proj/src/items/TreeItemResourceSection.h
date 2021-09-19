#ifndef TREEITEMRESOURCESECTION_H
#define TREEITEMRESOURCESECTION_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemResourceSection : public TreeItem
{
public:
	DWORD base;
	PIMAGE_NT_HEADERS pNTHeader;

	TreeItemResourceSection(TreeItem* parent, DWORD base, PIMAGE_NT_HEADERS pNTHeader) : TreeItem(parent)
	{
		this->base = base;
		this->pNTHeader = pNTHeader;
	}
	virtual QString getLabel() const
	{
		return "ResourceSection";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMRESOURCESECTION_H
