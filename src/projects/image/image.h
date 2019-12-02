/* __image_header_file__ */

#ifndef __IMAGE_H__
#define __IMAGE_H__

#ifdef _WIN32 // win32
	#include <stdio.h>
	#include <stdlib.h>
	#include <tchar.h>
	#include <windows.h>
	#include <winnt.h>
#else
	#error Current platform is not supported
#endif

#ifndef __IN
#define __IN
#endif

#ifndef __OUT
#define __OUT
#endif

#ifndef __OPT
#define __OPT
#endif

#define IMAGE_MAGIC_NUMBER		0x00000000 // Magic number

#define IMAGE_FILE_TYPE_PE		0x01 // Portable Executable
#define IMAGE_FILE_TYPE_ELF		0x02 // Executable and Linkable Format

#define IMAGE_ADDRESS(p)		((PVOID)((PUCHAR)p + p->SizeOfHeader))

// Image header
typedef struct _ImageHeader {

	// Magic number
	ULONG MagicNumber;

	UCHAR TypeOfImage;

	ULONG SizeOfHeader;
	ULONG SizeOfImage;

	UCHAR Hijack[1];

} ImageHeader, *PImageHeader;

PImageHeader ImageLoad(__IN PVOID Buffer);
BOOL ImageUnload(__IN PImageHeader Header);

#endif