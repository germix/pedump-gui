#include "TreeItemMiscDebugInfo.h"

QString DumpMiscDebugInfo(PIMAGE_DEBUG_MISC pMiscDebugInfo)
{
	QString str;

	if(IMAGE_DEBUG_MISC_EXENAME != pMiscDebugInfo->DataType)
	{
		str = QString().sprintf("Unknown Miscellaneous Debug Information type: %u\n", pMiscDebugInfo->DataType);
	}
	else
	{
		str = QString().sprintf("Miscellaneous Debug Information\n" );
		str = QString().sprintf("  %s\n", pMiscDebugInfo->Data );
	}

	return str;
}

QWidget* TreeItemMiscDebugInfo::getWidget() const
{
	return createTextEdit(DumpMiscDebugInfo(pMiscDebugInfo));
}
