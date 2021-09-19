#ifndef TREEITEMSYMBOLTABLE_H
#define TREEITEMSYMBOLTABLE_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemSymbolTable : public TreeItem
{
public:
	PCOFFSymbolTable pSymTab;

	TreeItemSymbolTable(TreeItem* parent, PCOFFSymbolTable pSymTab) : TreeItem(parent)
	{
		this->pSymTab = pSymTab;
	}
	virtual QString getLabel() const
	{
		return "SymbolTable";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMSYMBOLTABLE_H
