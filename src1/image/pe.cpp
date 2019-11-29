/* __pe_source__ */

#include "pe.h"

/************************************************************************/
/* 获取PE DOS                                                           */
/************************************************************************/
PIMAGE_DOS_HEADER pe_dos_header(__in pvoid addr)
{
	return (PIMAGE_DOS_HEADER)addr;
}

/************************************************************************/
/* 获取PE NT                                                            */
/************************************************************************/
PIMAGE_NT_HEADERS pe_nt_header(__in pvoid addr)
{
	return (PIMAGE_NT_HEADERS)((puchar)addr + pe_dos_header(addr)->e_lfanew);
}

/************************************************************************/
/* 获取PE FILE                                                          */
/************************************************************************/
PIMAGE_FILE_HEADER pe_file_header(__in pvoid addr)
{
	return & pe_nt_header(addr)->FileHeader;
}

/************************************************************************/
/* 获取PE OPTIONAL                                                      */
/************************************************************************/
PIMAGE_OPTIONAL_HEADER pe_optional_header(__in pvoid addr)
{
	return & pe_nt_header(addr)->OptionalHeader;
}

/************************************************************************/
/* 获取DATA DIRECTORY                                                   */
/************************************************************************/
PIMAGE_DATA_DIRECTORY pe_data_directory(__in pvoid addr, __in uint index)
{
	return & pe_optional_header(addr)->DataDirectory[index];
}

/************************************************************************/
/* 获取SECTION HEADER                                                   */
/************************************************************************/
PIMAGE_SECTION_HEADER pe_section_header(__in pvoid addr, __in uint index)
{
	return & IMAGE_FIRST_SECTION(pe_nt_header(addr))[index];
}

/************************************************************************/
/* 检查是否合法PE文件                                                   */
/************************************************************************/
bool pe_is(__in pvoid addr)
{
	return (pe_dos_header(addr)->e_magic == IMAGE_DOS_SIGNATURE && pe_nt_header(addr)->Signature == IMAGE_NT_SIGNATURE);
}

/************************************************************************/
/* 检查是否DLL文件                                                      */
/************************************************************************/
bool pe_is_dll(__in pvoid addr)
{
	return (pe_file_header(addr)->Characteristics & IMAGE_FILE_DLL) == IMAGE_FILE_DLL;
}