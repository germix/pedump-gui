#ifndef TREEITEMSECTION_H
#define TREEITEMSECTION_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemSection : public TreeItem
{
public:
	DWORD base;
	int index;
	QString name;
	PIMAGE_SECTION_HEADER section;

	TreeItemSection(TreeItem* parent, DWORD base, int index, const char* name, PIMAGE_SECTION_HEADER section) : TreeItem(parent)
	{
		this->base = base;
		this->index = index;
		this->name = name;
		this->section = section;
	}
	virtual QString getLabel() const
	{
		return name;
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMSECTION_H
