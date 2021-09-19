#include "TreeItemBoundImportDescriptors.h"

//
// Dump out the new IMAGE_BOUND_IMPORT_DESCRIPTOR that NT 3.51 added
//
QString DumpBoundImportDescriptors(DWORD base, PIMAGE_NT_HEADERS pNTHeader)
{
	QString str;

	DWORD bidRVA; // Bound import descriptors RVA
	PIMAGE_BOUND_IMPORT_DESCRIPTOR pibid;

	bidRVA = GetImgDirEntryRVA(pNTHeader, IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT);
	if(!bidRVA)
		return "";

	pibid = MakePtr( PIMAGE_BOUND_IMPORT_DESCRIPTOR, base, bidRVA);

	str += QString().sprintf("Bound import descriptors:\n\n" );
	str += QString().sprintf("  Module        TimeDate\n" );
	str += QString().sprintf("  ------------  --------\n" );

	while(pibid->TimeDateStamp)
	{
		PIMAGE_BOUND_FORWARDER_REF pibfr;

		str += QString().sprintf("  %-12s  %08X -> %s",
				base + bidRVA + pibid->OffsetModuleName,
				pibid->TimeDateStamp,
				ctime((time_t *)&pibid->TimeDateStamp) );

		pibfr = MakePtr(PIMAGE_BOUND_FORWARDER_REF, pibid, sizeof(IMAGE_BOUND_IMPORT_DESCRIPTOR));

		for(unsigned int i = 0; i < pibid->NumberOfModuleForwarderRefs; i++)
		{
			str += QString().sprintf("    forwarder:  %-12s  %08X -> %s",
					base + bidRVA + pibfr->OffsetModuleName,
					pibfr->TimeDateStamp,
					ctime((time_t *)&pibfr->TimeDateStamp));

			pibfr++; // advance to next forwarder ref

			// Keep the outer loop pointer up to date too!
			pibid = MakePtr(PIMAGE_BOUND_IMPORT_DESCRIPTOR, pibid, sizeof(IMAGE_BOUND_FORWARDER_REF));
		}

		pibid++; // Advance to next pibid;
	}

	return str;
}

QWidget* TreeItemBoundImportDescriptors::getWidget() const
{
	return createTextEdit(DumpBoundImportDescriptors(base, pNTHeader));
}
