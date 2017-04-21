/* __image_header__ */

#ifndef __IMAGE_INNER_H__
#define __IMAGE_INNER_H__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <Windows.h>
#include <WinNT.h>
#else
#error Current platform is not supported
#endif


//////////////////////////////////////////////////////////////////////////
// Macro
#ifndef __in
#define __in
#endif

#ifndef __out
#define __out
#endif

#ifndef __opt
#define __opt
#endif

#ifndef __syscall
#define __syscall	__stdcall
#endif

#ifndef __naked
#define __naked		__declspec(naked)
#endif

#ifndef __import
#define __import	__declspec(dllimport)
#endif

#ifndef __export
#define __export	__declspec(dllexport)
#endif

typedef void				nil,	*pvoid;
typedef unsigned char		uchar,  *puchar;
typedef uchar				byte,	*pbyte;
typedef unsigned short		ushort, *pushort;
typedef unsigned int		uint,	*puint;
typedef unsigned long		ulong,  *pulong;

#ifndef null
#define null	0
#endif

#ifndef PAGE_SIZE
#define PAGE_SIZE	4096
#endif

#ifndef _W64
#define _W64
#endif

#define IMAGE_TRAP_MAX			20
#define IMAGE_TRAP_TEXT_SIZE	256

typedef pvoid (__syscall *malloc_t)	(ulong);
typedef void  (__syscall *free_t)	(pvoid);

#pragma pack(push, 4)

typedef struct _image_apis {
	int (WINAPI *MessageBoxA)	(HWND, LPCSTR, LPCSTR, UINT);
	int (WINAPI *MessageBoxW)	(HWND, LPCWSTR, LPCWSTR, UINT);
	int (WINAPI *lstrcmpiA)		(LPCSTR, LPCSTR);
	int (WINAPI *lstrcmpiW)		(LPCWSTR, LPCWSTR);
} image_apis;

typedef struct _image_hook {
	LIST_ENTRY head;
	pvoid *address;
	pvoid  function;
} image_hook;

typedef struct _image_trap {
	pvoid proc;
	pvoid trap;
	ulong size;
} image_trap;

typedef struct _image_trap_text {
	byte text[IMAGE_TRAP_TEXT_SIZE];
} image_trap_text;

typedef struct _image_information {

	// size of image information
	ulong size;

	// image
	pvoid imagebase;
	ulong imagesize;

	struct {
		CHAR	A[32];
		WCHAR	W[32];
	} modulebasename;

	struct {
		CHAR	A[32];
		WCHAR	W[32];
	} modulename;

	struct {
		CHAR	A[260];
		WCHAR	W[260];
	} modulepath;

	ulong LengthOfName;
	ulong LengthOfPath;

	// sys apis
	image_apis apis;

	// traps
	image_trap traps[IMAGE_TRAP_MAX];

	// hooks
	image_hook	*hook;

	// memory manager
	struct {
		malloc_t	malloc;
		free_t		free;
	} mm;

	// input
	struct {
		pvoid buffer;
		ulong size;
	} in;

	// 中间过程代码存储
	image_trap_text text[1];

} image_information;

#pragma pack(pop)

#define TO_IMAGE(info)	image_to_im(info)
#define TO_INFO(addr)	image_to_information(addr)

#define	image_alignment(size, alignment) \
	(size % alignment ? (((size / alignment) + 1) * alignment) : size)

pvoid				image_load				(__in pvoid buffer, __in ulong size, __in malloc_t m = null, __in free_t f = null);
void				image_free				(__in pvoid addr);
pvoid				image_proc				(__in pvoid addr, __in const char *name);

bool				image_init				(__out image_information *info, __in pvoid buffer, __in ulong size, __in malloc_t m, __in free_t f);
pvoid				image_load				(__in image_information *info);

bool				image_init_apis			(__in image_information *info);
bool				image_init_traps		(__in image_information *info);

ulong				image_sizeof			(__in pvoid addr);

bool				image_relocation		(__in pvoid addr,	 __in image_information *info);
bool				image_fix_import		(__in pvoid addr,	 __in image_information *info);
bool				image_fix_import_for	(__in pvoid hModule, __in image_information *info);

pvoid				image_copy_im			(__in image_information *info);
void				image_copy_traps		(__in image_information *info);

pvoid				image_to_im				(__in image_information *info);
image_information*	image_to_information	(__in pvoid addr);

image_hook*			image_hook_push			(__in image_information *info, __in pvoid *address, __in pvoid function);
bool				image_hook_pop			(__in image_information *info, __out image_hook *hook);

#endif