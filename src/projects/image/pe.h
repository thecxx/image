/* __pe_header_file__ */

#ifndef __PE_H__
#define __PE_H__

#include "image.h"

#define IMAGE_HIJACK_GET_MODULE_HANDLE_A
#define IMAGE_HIJACK_GET_MODULE_HANDLE_W
#define IMAGE_HIJACK_GET_MODULE_FILENAME_A
#define IMAGE_HIJACK_GET_MODULE_FILENAME_W

#define ImageNumberOfHijack	PeNumberOfHijack
#define ImageInitHijack     PeInitHijack
#define ImageIsValid		PeIsValid
#define ImageSizeOfImage	PeSizeOfImage
#define ImageAlloc			PeAlloc
#define ImageFree			PeFree

PIMAGE_DOS_HEADER		PeDosHeader(__IN PVOID Addr);
PIMAGE_NT_HEADERS		PeNtHeader(__IN PVOID Addr);
PIMAGE_FILE_HEADER		PeFileHeader(__IN PVOID Addr);
PIMAGE_OPTIONAL_HEADER	PeOptionalHeader(__IN PVOID Addr);
PIMAGE_SECTION_HEADER	PeSectionHeader(__IN PVOID Addr, __IN UINT Index);

BOOL	PeIsValid(__IN PVOID Addr, __IN USHORT Machine, __IN USHORT Characteristics);
ULONG	PeSectionNumber(__IN PVOID Addr);
ULONG	PeSizeOfImage(__IN PVOID Addr);
ULONG	PeAlignment(__IN ULONG Size, __IN ULONG Alignment);
ULONG	PeNumberOfHijack();
BOOL    PeInitHijack(__IN PImageHijack hijack, __IN UINT Index);
PVOID	PeAlloc(__IN Size);
VOID	PeFree(__IN PVOID Ptr);

#endif
