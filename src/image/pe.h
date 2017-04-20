/* __pe_header__ */

#ifndef __PE_H__
#define __PE_H__

#include "image.h"

typedef WORD reloc_block;
typedef BOOL (__syscall *dllmain_t)(__in pvoid, __in ulong, __in pvoid);

#define PE_SECTION(addr, index)		(pe_section_header(addr, index))
#define PE_SECTION_COUNT(addr)		(pe_file_header	  (addr)->NumberOfSections)

PIMAGE_DOS_HEADER		pe_dos_header		(__in pvoid addr);
PIMAGE_NT_HEADERS		pe_nt_header		(__in pvoid addr);
PIMAGE_FILE_HEADER		pe_file_header		(__in pvoid addr);
PIMAGE_OPTIONAL_HEADER	pe_optional_header	(__in pvoid addr);

PIMAGE_DATA_DIRECTORY	pe_data_directory	(__in pvoid addr, __in uint index);
PIMAGE_SECTION_HEADER	pe_section_header	(__in pvoid addr, __in uint index);

bool					pe_is				(__in pvoid addr);
bool					pe_is_dll			(__in pvoid addr);

template<class T>
T pe_entry(__in pvoid addr)
{
	return (T)((puchar)addr + pe_optional_header(addr)->AddressOfEntryPoint);
}

template<class T>
T pe_data_directory(__in pvoid addr, __in uint index, __out __opt ulong *size)
{
	PIMAGE_DATA_DIRECTORY directory = pe_data_directory(addr, index);
	if(directory->VirtualAddress == null) {
		return (T)null;
	} else if(size) {
		*size = directory->Size;
	}
	return (T)((puchar)addr + directory->VirtualAddress);
}

#endif