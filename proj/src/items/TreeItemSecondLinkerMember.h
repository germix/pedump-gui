#ifndef TREEITEMSECONDLINKERMEMBER_H
#define TREEITEMSECONDLINKERMEMBER_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemSecondLinkerMember : public TreeItem
{
public:
	PVOID p;

	TreeItemSecondLinkerMember(TreeItem* parent, PVOID p) : TreeItem(parent)
	{
		this->p = p;
	}
	virtual QString getLabel() const
	{
		return "SecondLinkerMember";
	}
	virtual QWidget* getWidget() const;
};


#endif // TREEITEMSECONDLINKERMEMBER_H