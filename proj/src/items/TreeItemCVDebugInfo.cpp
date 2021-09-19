#include "TreeItemCVDebugInfo.h"

QString DumpCVDebugInfo(PDWORD pCVHeader)
{
	QString str;
	PPDB_INFO pPDBInfo;

	str += QString().sprintf("CodeView Signature: %08X\n", *pCVHeader);

	if('01BN' != *pCVHeader)
	{
		str += QString().sprintf("Unhandled CodeView Information format %.4s\n", pCVHeader);
	}
	else
	{
		pPDBInfo = (PPDB_INFO)pCVHeader;

		str += QString().sprintf("  Offset: %08X  Signature: %08X  Age: %08X\n",
				pPDBInfo->Offset,
				pPDBInfo->sig,
				pPDBInfo->age);

		str += QString().sprintf("  File: %s\n", pPDBInfo->PdbName);
	}

	return str;
}

QWidget* TreeItemCVDebugInfo::getWidget() const
{
	return createTextEdit(DumpCVDebugInfo(pCVHeader));
}
