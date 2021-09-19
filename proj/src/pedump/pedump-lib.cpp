#include "pedump.h"
#include <QMessageBox>

#include "items/TreeItemArchiveMemberHeader.h"
#include "items/TreeItemFirstLinkerMember.h"
#include "items/TreeItemSecondLinkerMember.h"
#include "items/TreeItemLongnamesMember.h"

void PEDumpModel::loadLibFile(TreeItem* parentItem, LPVOID lpFileBase)
{
	PIMAGE_ARCHIVE_MEMBER_HEADER pArchHeader;
	BOOL fSawFirstLinkerMember = FALSE;
	BOOL fSawSecondLinkerMember = FALSE;

	if(strncmp((char*)lpFileBase,IMAGE_ARCHIVE_START, IMAGE_ARCHIVE_START_SIZE))
	{
		QMessageBox::information(nullptr, tr("Error"), tr("Not a valid .LIB file - signature not found\n"), QMessageBox::Ok);
		return;
	}

	pArchHeader = MakePtr(PIMAGE_ARCHIVE_MEMBER_HEADER, lpFileBase, IMAGE_ARCHIVE_START_SIZE);

	PCSTR PszLongnames;
	while(pArchHeader)
	{
		DWORD thisMemberSize;

		new TreeItemArchiveMemberHeader(parentItem, PszLongnames, pArchHeader, (PBYTE)pArchHeader - (PBYTE) lpFileBase);

		if(!strncmp((char*)pArchHeader->Name, IMAGE_ARCHIVE_LINKER_MEMBER, 16))
		{
			if(!fSawFirstLinkerMember)
			{
				new TreeItemFirstLinkerMember(parentItem, (PVOID)(pArchHeader + 1));
				fSawFirstLinkerMember = TRUE;
			}
			else if(!fSawSecondLinkerMember)
			{
				new TreeItemSecondLinkerMember(parentItem, (PVOID)(pArchHeader + 1));
				fSawSecondLinkerMember = TRUE;
			}
		}
		else if(!strncmp((char*)pArchHeader->Name, IMAGE_ARCHIVE_LONGNAMES_MEMBER, 16))
		{
			// Save off pointer for use when dumping out OBJ member names
			PszLongnames = (PCSTR)(PVOID)(pArchHeader + 1);

			new TreeItemLongnamesMember(parentItem, (LPVOID)PszLongnames, atoi((char*)pArchHeader->Size));
		}
		else // It's an OBJ file
		{
			loadObjFile(parentItem, (PIMAGE_FILE_HEADER)(pArchHeader + 1) );
		}

		// Calculate how big this member is (it's originally stored as
		// as ASCII string.
		thisMemberSize = atoi((char*)pArchHeader->Size) + IMAGE_SIZEOF_ARCHIVE_MEMBER_HDR;

		thisMemberSize = (thisMemberSize+1) & ~1; // Round up

		// Get a pointer to the next archive member
		pArchHeader = MakePtr(PIMAGE_ARCHIVE_MEMBER_HEADER, pArchHeader, thisMemberSize);

		if(strncmp((char*)pArchHeader->EndHeader, IMAGE_ARCHIVE_END, 2))
			break;
	}
}
