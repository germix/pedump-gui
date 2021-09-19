#ifndef TREEITEMBOUNDIMPORTDESCRIPTORS_H
#define TREEITEMBOUNDIMPORTDESCRIPTORS_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemBoundImportDescriptors : public TreeItem
{
public:
	DWORD base;
	PIMAGE_NT_HEADERS pNTHeader;

	TreeItemBoundImportDescriptors(TreeItem* parent, DWORD base, PIMAGE_NT_HEADERS pNTHeader) : TreeItem(parent)
	{
		this->base = base;
		this->pNTHeader = pNTHeader;
	}
	virtual QString getLabel() const
	{
		return "BoundImportDescriptors";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMBOUNDIMPORTDESCRIPTORS_H
