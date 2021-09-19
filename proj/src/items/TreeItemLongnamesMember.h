#ifndef TREEITEMLONGNAMESMEMBER_H
#define TREEITEMLONGNAMESMEMBER_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemLongnamesMember : public TreeItem
{
public:
	PVOID p;
	DWORD len;

	TreeItemLongnamesMember(TreeItem* parent, PVOID p, DWORD len) : TreeItem(parent)
	{
		this->p = p;
		this->len = len;
	}
	virtual QString getLabel() const
	{
		return "LongnamesMember";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMLONGNAMESMEMBER_H