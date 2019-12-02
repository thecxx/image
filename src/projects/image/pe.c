/* __pe_source_file__ */

#include "pe.h"

PIMAGE_DOS_HEADER PeDosHeader(__IN PVOID Addr)
{
	return (PIMAGE_DOS_HEADER)Addr;
}

PIMAGE_NT_HEADERS PeNtHeader(__IN PVOID Addr)
{
	return (PIMAGE_NT_HEADERS)((PUCHAR)Addr + PeDosHeader(Addr)->e_lfanew);
}

BOOL PeValidate(__IN PVOID Addr)
{
	return PeDosHeader(Addr)->e_magic == IMAGE_DOS_SIGNATURE && PeNtHeader(Addr)->Signature == IMAGE_NT_SIGNATURE;
}