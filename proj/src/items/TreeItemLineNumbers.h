#ifndef TREEITEMLINENUMBERS_H
#define TREEITEMLINENUMBERS_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemLineNumbers : public TreeItem
{
public:
	COFFSymbolTable* pCOFFSymbolTable;
	PIMAGE_LINENUMBER pln;
	DWORD count;

	TreeItemLineNumbers(TreeItem* parent, COFFSymbolTable* pCOFFSymbolTable, PIMAGE_LINENUMBER pln, DWORD count) : TreeItem(parent)
	{
		this->pCOFFSymbolTable = pCOFFSymbolTable;
		this->pln = pln;
		this->count = count;
	}
	virtual QString getLabel() const
	{
		return "LineNumbers";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMLINENUMBERS_H
