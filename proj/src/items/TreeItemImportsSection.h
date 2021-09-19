#ifndef TREEITEMIMPORTSSECTION_H
#define TREEITEMIMPORTSSECTION_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemImportsSection : public TreeItem
{
public:
	DWORD base;
	PIMAGE_NT_HEADERS pNTHeader;

	TreeItemImportsSection(TreeItem* parent, DWORD base, PIMAGE_NT_HEADERS pNTHeader) : TreeItem(parent)
	{
		this->base = base;
		this->pNTHeader = pNTHeader;
	}
	virtual QString getLabel() const
	{
		return "ImportsSection";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMIMPORTSSECTION_H
