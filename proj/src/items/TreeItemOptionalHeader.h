#ifndef TREEITEMOPTIONALHEADER_H
#define TREEITEMOPTIONALHEADER_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemOptionalHeader : public TreeItem
{
public:
	PIMAGE_OPTIONAL_HEADER optionalHeader;

	TreeItemOptionalHeader(TreeItem* parent, PIMAGE_OPTIONAL_HEADER optionalHeader) : TreeItem(parent)
	{
		this->optionalHeader = optionalHeader;
	}
	virtual QString getLabel() const
	{
		return "OptionalHeader";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMOPTIONALHEADER_H
