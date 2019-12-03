/* __image_header_file__ */

#ifndef __IMAGE_H__
#define __IMAGE_H__

#if defined(_WIN32) || defined(_WIN64) // windows
	#include <stdio.h>
	#include <stdlib.h>
	#include <tchar.h>
	#include <windows.h>
	#include <winnt.h>

	#ifdef _WIN32 // win32
		#define IMAGE_FILE_MACHINE_SUPPORTED	IMAGE_FILE_MACHINE_I386
	#else		  // win64
		#define IMAGE_FILE_MACHINE_SUPPORTED	IMAGE_FILE_MACHINE_AMD64
	#endif
	#define IMAGE_FILE_TYPE_DYNAMIC_LIBRARY		IMAGE_FILE_DLL

#elif defined(_LINUX)
	#error Current platform is not supported
#else
	#error Current platform is not supported
#endif

#ifndef __IN
#define __IN
#endif

#ifndef __OUT
#define __OUT
#endif

#ifndef __IN_OPT
#define __IN_OPT
#endif

#ifndef __OUT_OPT
#define __OUT_OPT
#endif

#ifndef PAGE_SIZE
#define PAGE_SIZE	4096
#endif

#ifndef POINTER_SIZE
#define POINTER_SIZE	sizeof(void*)
#endif

#define IMAGE_HEADER_MAGIC_SIGNATURE	0x00000000 // Magic number

// Error codes
#define IMAGE_ERROR_NONE				0x00
#define IMAGE_ERROR_INVALID_HEADER		0x01
#define IMAGE_ERROR_INVALID_FILE		0x02
#define IMAGE_ERROR_INVALID_FILE_SIZE	0x03

#define IMAGE_ADDRESS(h)				((PVOID)((PUCHAR)h + h->SizeOfHeader))
#define IMAGE_FIRST_HIJACK(h)			((PImageHijack)((PUCHAR)h + sizeof(ImageHeader)))

#define RETURN_SUCCESS(val)				\
{ \
	return val; \
}
#define RETURN_FAIL(val, err)			\
{ \
	return val; \
}

// Image header
typedef struct _ImageHeader {

	// Magic number
	ULONG MagicNumber;

	ULONG SizeOfHeader;
	ULONG SizeOfImage;

	ULONG NumberOfHijack;

} ImageHeader, *PImageHeader;

// Image hijack
typedef struct _ImageHijack {
	PUCHAR Buffer[1024];
} ImageHijack, *PImageHijack;

PImageHeader ImageLoad(__IN PVOID Buffer, __IN USHORT Machine, __IN USHORT FileType);
BOOL		 ImageUnload(__IN PImageHeader Header);

#endif