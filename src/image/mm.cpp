/* __mm_source__ */

#include "mm.h"

/************************************************************************/
/* 分配内存                                                             */
/************************************************************************/
pvoid __syscall mm_malloc(__in ulong size)
{
	return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}

/************************************************************************/
/* 释放内存                                                             */
/************************************************************************/
void __syscall mm_free(__in pvoid addr)
{
	VirtualFree(addr, 0, MEM_RELEASE);
}

bool __syscall mm_protect(__in pvoid addr, __in ulong size, __in ulong new_protect, __out ulong *old_protect)
{
	return (VirtualProtect(addr, size, new_protect, old_protect) == TRUE);
}