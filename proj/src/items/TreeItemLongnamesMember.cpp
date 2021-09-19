#include "TreeItemLongnamesMember.h"

QWidget* TreeItemLongnamesMember::getWidget() const
{
	QString str;

	PSTR pszName = (PSTR)p;
	DWORD offset = 0;

	str += QString().sprintf("Longnames:\n");

	// The longnames member is a series of null-terminated string.  Print
	// out the offset of each string (in decimal), followed by the string.
	while(offset < len)
	{
		unsigned int cbString = strlen(pszName) + 1;

		str += QString().sprintf("  %05u: %s\n", offset, pszName);
		offset += cbString;
		pszName += cbString;
	}

	return createTextEdit(str);
}
