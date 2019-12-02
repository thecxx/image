/* __pe_header_file__ */

#ifndef __PE_H__
#define __PE_H__

#include "image.h"

PIMAGE_DOS_HEADER PeDosHeader(__IN PVOID Addr);
PIMAGE_NT_HEADERS PeNtHeader(__IN PVOID Addr);

BOOL PeValidate(__IN PVOID Addr);

#endif
