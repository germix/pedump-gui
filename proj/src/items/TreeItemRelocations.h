#ifndef TREEITEMRELOCATIONS_H
#define TREEITEMRELOCATIONS_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemRelocations : public TreeItem
{
public:
	PIMAGE_FILE_HEADER pImageFileHeader;
	PIMAGE_SECTION_HEADER pSections;

	TreeItemRelocations(TreeItem* parent, PIMAGE_FILE_HEADER pImageFileHeader, PIMAGE_SECTION_HEADER pSections) : TreeItem(parent)
	{
		this->pImageFileHeader = pImageFileHeader;
		this->pSections = pSections;
	}
	virtual QString getLabel() const
	{
		return "Relocations";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMRELOCATIONS_H
