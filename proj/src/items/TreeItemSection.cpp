#include "TreeItemSection.h"

// Number of hex values displayed per line
#define HEX_DUMP_WIDTH 16

//
// Dump a region of memory in a hexadecimal format
//
QString HexDump(PBYTE ptr, DWORD length)
{
	QString str;
	char buffer[256];
	PSTR buffPtr, buffPtr2;
	unsigned cOutput, i;
	DWORD bytesToGo = length;

	while(bytesToGo)
	{
		cOutput = bytesToGo >= HEX_DUMP_WIDTH ? HEX_DUMP_WIDTH : bytesToGo;

		buffPtr = buffer;
		buffPtr += sprintf(buffPtr, "%08X:  ", length-bytesToGo);
		buffPtr2 = buffPtr + (HEX_DUMP_WIDTH * 3) + 1;

		for(i = 0; i < HEX_DUMP_WIDTH; i++)
		{
			BYTE value = *(ptr+i);

			if(i >= cOutput)
			{
				// On last line.  Pad with spaces
				*buffPtr++ = ' ';
				*buffPtr++ = ' ';
				*buffPtr++ = ' ';
			}
			else
			{
				if(value < 0x10)
				{
					*buffPtr++ = '0';
					itoa( value, buffPtr++, 16);
				}
				else
				{
					itoa( value, buffPtr, 16);
					buffPtr+=2;
				}

				*buffPtr++ = ' ';
				*buffPtr2++ = isprint(value) ? value : '.';
			}

			// Put an extra space between the 1st and 2nd half of the bytes
			// on each line.
			if(i == (HEX_DUMP_WIDTH/2)-1)
				*buffPtr++ = ' ';
		}

		*buffPtr2++ = '\n';
		*buffPtr2++ = 0;
		str += buffer;
		bytesToGo -= cOutput;
		ptr += HEX_DUMP_WIDTH;
	}

	return str;
}

QWidget* TreeItemSection::getWidget() const
{
	QString str;

	str += QString().sprintf("section %02X (%ls)  size: %08X  file offs: %08X\n",
			index,
			name.constData(),
			section->SizeOfRawData,
			section->PointerToRawData);

	if(section->PointerToRawData != 0)
	{
		str += HexDump(MakePtr(PBYTE, base, section->PointerToRawData), section->SizeOfRawData);
	}
	return createTextEdit(str);
}
