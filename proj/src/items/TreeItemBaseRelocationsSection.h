#ifndef TREEITEMBASERELOCATIONSSECTION_H
#define TREEITEMBASERELOCATIONSSECTION_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemBaseRelocationsSection : public TreeItem
{
public:
	DWORD base;
	PIMAGE_NT_HEADERS pNTHeader;

	TreeItemBaseRelocationsSection(TreeItem* parent, DWORD base, PIMAGE_NT_HEADERS pNTHeader) : TreeItem(parent)
	{
		this->base = base;
		this->pNTHeader = pNTHeader;
	}
	virtual QString getLabel() const
	{
		return "BaseRelocationsSection";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMBASERELOCATIONSSECTION_H
