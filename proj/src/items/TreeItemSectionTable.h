#ifndef TREEITEMSECTIONTABLE_H
#define TREEITEMSECTIONTABLE_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemSectionTable : public TreeItem
{
public:
	PIMAGE_SECTION_HEADER section;
	unsigned int cSections;
	BOOL IsEXE;

	TreeItemSectionTable(TreeItem* parent, DWORD base, PIMAGE_SECTION_HEADER section, unsigned int cSections, BOOL IsEXE);

	virtual QString getLabel() const
	{
		return "SectionTable";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMSECTIONTABLE_H
