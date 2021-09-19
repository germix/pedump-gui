#ifndef TREEITEMFIRSTLINKERMEMBER_H
#define TREEITEMFIRSTLINKERMEMBER_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemFirstLinkerMember : public TreeItem
{
public:
	PVOID p;

	TreeItemFirstLinkerMember(TreeItem* parent, PVOID p) : TreeItem(parent)
	{
		this->p = p;
	}
	virtual QString getLabel() const
	{
		return "FirstLinkerMember";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMFIRSTLINKERMEMBER_H
