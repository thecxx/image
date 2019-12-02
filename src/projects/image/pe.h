/* __pe_header_file__ */

#ifndef __PE_H__
#define __PE_H__

#include "image.h"

// Machine
#ifdef _WIN32
	#define IMAGE_FILE_MACHINE_SUPPORTED			IMAGE_FILE_MACHINE_I386
#elif defined(_WIN64)
	#define IMAGE_FILE_MACHINE_SUPPORTED			IMAGE_FILE_MACHINE_AMD64
#else
	#error Current platform is not supported
#endif

// Dll
#define IMAGE_FILE_DYNAMIC_LIBRARY					IMAGE_FILE_DLL


#define ImageIsValid PeIsValid


PIMAGE_DOS_HEADER PeDosHeader(__IN PVOID Addr);
PIMAGE_NT_HEADERS PeNtHeader(__IN PVOID Addr);

BOOL PeIsValid(__IN PVOID Addr, __IN USHORT Machine, __IN USHORT Characteristics);

#endif
