/* __ldr_header__ */

#ifndef __LDR_H__
#define __LDR_H__

#ifdef _DEBUG
#pragma comment(lib, "image.d.lib")
#else
#pragma comment(lib, "image.lib")
#endif

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <Windows.h>
#include <WinNT.h>
#else
#error Current platform is not supported
#endif

typedef PVOID (__stdcall *malloc_t)	(ULONG);
typedef VOID  (__stdcall *free_t)	(PVOID);

PVOID	LdrLoadImage		(PVOID Buffer, DWORD Size, malloc_t m = NULL, free_t f = NULL);
PVOID	LdrGetProcAddress	(PVOID Addr, LPCSTR Name);
VOID	LdrFreeImage		(PVOID Addr);

#endif