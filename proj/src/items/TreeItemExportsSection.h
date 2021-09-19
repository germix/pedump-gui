#ifndef TREEITEMEXPORTSSECTION_H
#define TREEITEMEXPORTSSECTION_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemExportsSection : public TreeItem
{
public:
	DWORD base;
	PIMAGE_NT_HEADERS pNTHeader;

	TreeItemExportsSection(TreeItem* parent, DWORD base, PIMAGE_NT_HEADERS pNTHeader) : TreeItem(parent)
	{
		this->base = base;
		this->pNTHeader = pNTHeader;
	}
	virtual QString getLabel() const
	{
		return "ExportsSection";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMEXPORTSSECTION_H
