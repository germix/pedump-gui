#include "TreeItemArchiveMemberHeader.h"

QWidget* TreeItemArchiveMemberHeader::getWidget() const
{
	QString str;

	str += QString().sprintf("Archive Member Header (%08X):\n", fileOffset);

	str += QString().sprintf("  Name:     %.16s", pArchHeader->Name);
	if(pArchHeader->Name[0] == '/' && isdigit(pArchHeader->Name[1]))
	{
		str += QString().sprintf("  (%s)\n", PszLongnames + atoi((char *)pArchHeader->Name+1));
	}
	str += QString().sprintf("\n");

	char szDateAsLong[64];
	sprintf(szDateAsLong, "%.12s", pArchHeader->Date);
	LONG dateAsLong = atol(szDateAsLong);

	str += QString().sprintf("  Date:     %.12s %s", pArchHeader->Date, ctime((time_t *) &dateAsLong));
	str += QString().sprintf("  UserID:   %.6s\n", pArchHeader->UserID);
	str += QString().sprintf("  GroupID:  %.6s\n", pArchHeader->GroupID);
	str += QString().sprintf("  Mode:     %.8s\n", pArchHeader->Mode);
	str += QString().sprintf("  Size:     %.10s\n", pArchHeader->Size);

	return createTextEdit(str);
}
