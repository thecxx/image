/* __mm_header__ */

#ifndef __MM_H__
#define __MM_H__

#include "image.h"

pvoid __syscall mm_malloc	(__in ulong size);
void  __syscall mm_free		(__in pvoid addr);
bool  __syscall mm_protect	(__in pvoid addr, __in ulong size, __in ulong new_protect, __out ulong *old_protect);

#endif