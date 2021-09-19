#ifndef TREEITEMRUNTIMEFUNCTIONS_H
#define TREEITEMRUNTIMEFUNCTIONS_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemRuntimeFunctions : public TreeItem
{
public:
	COFFSymbolTable* pCOFFSymbolTable;
	DWORD base;
	PIMAGE_NT_HEADERS pNTHeader;

	TreeItemRuntimeFunctions(TreeItem* parent, COFFSymbolTable* pCOFFSymbolTable, DWORD base, PIMAGE_NT_HEADERS pNTHeader) : TreeItem(parent)
	{
		this->pCOFFSymbolTable = pCOFFSymbolTable;
		this->base = base;
		this->pNTHeader = pNTHeader;
	}
	virtual QString getLabel() const
	{
		return "RuntimeFunctions";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMRUNTIMEFUNCTIONS_H
