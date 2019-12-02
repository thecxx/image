/* __pe_source_file__ */

#include "pe.h"

// Locals
PIMAGE_FILE_HEADER PeFileHeader(__IN PVOID Addr);
PIMAGE_OPTIONAL_HEADER PeOptionalHeader(__IN PVOID Addr);


PIMAGE_DOS_HEADER PeDosHeader(__IN PVOID Addr)
{
	return (PIMAGE_DOS_HEADER)Addr;
}

PIMAGE_NT_HEADERS PeNtHeader(__IN PVOID Addr)
{
	return (PIMAGE_NT_HEADERS)((PUCHAR)Addr + PeDosHeader(Addr)->e_lfanew);
}

PIMAGE_FILE_HEADER PeFileHeader(__IN PVOID Addr)
{
	return & PeNtHeader(Addr)->FileHeader;
}

PIMAGE_OPTIONAL_HEADER PeOptionalHeader(__IN PVOID Addr)
{
	return & PeNtHeader(Addr)->OptionalHeader;
}

BOOL PeIsValid(__IN PVOID Addr, __IN USHORT Machine, __IN USHORT Characteristics)
{
	if(PeDosHeader(Addr)->e_magic != IMAGE_DOS_SIGNATURE || PeNtHeader(Addr)->Signature != IMAGE_NT_SIGNATURE) {
		return FALSE;
	}
	if((PeFileHeader(Addr)->Machine & Machine) != Machine) {
		return FALSE;
	}
	if((PeFileHeader(Addr)->Characteristics & Characteristics) != Characteristics) {
		return FALSE;
	}
	return TRUE;
}