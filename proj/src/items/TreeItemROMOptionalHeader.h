#ifndef TREEITEMROMOPTIONALHEADER_H
#define TREEITEMROMOPTIONALHEADER_H
#include "pedump/pedump.h"
#include "TreeItem.h"

class TreeItemROMOptionalHeader : public TreeItem
{
public:
	PIMAGE_ROM_OPTIONAL_HEADER pROMOptHdr;

	TreeItemROMOptionalHeader(TreeItem* parent, PIMAGE_ROM_OPTIONAL_HEADER pROMOptHdrIn) : TreeItem(parent)
	{
		this->pROMOptHdr = pROMOptHdrIn;
	}
	virtual QString getLabel() const
	{
		return "ROMOptionalHeader";
	}
	virtual QWidget* getWidget() const;
};

#endif // TREEITEMROMOPTIONALHEADER_H
