/* __stub_header__ */

#ifndef __STUB_H__
#define __STUB_H__

#include "image.h"

#ifdef __cplusplus
extern "C"
{
#endif

HMODULE __syscall GetModuleHandleAStub(__in __opt LPCSTR lpModuleName);
HMODULE __syscall GetModuleHandleWStub(__in __opt LPCWSTR lpModuleName);
DWORD   __syscall GetModuleFileNameAStub(__in __opt HMODULE hModule, __out LPSTR lpFilename, __in DWORD nSize);
DWORD   __syscall GetModuleFileNameWStub(__in __opt HMODULE hModule, __out LPWSTR lpFilename, __in DWORD nSize);
FARPROC __syscall GetProcAddressStub(__in HMODULE hModule, __in LPCSTR lpProcName);

#ifdef __cplusplus
};
#endif

#pragma pack(push, 1)

typedef struct _asm_jmp {
	uchar	opcode;
	ulong	operand;	
} asm_jmp;

typedef struct _asm_mov_esi {
	uchar	opcode;
	ulong	operand;
} asm_mov_esi;

#pragma pack(pop)

typedef struct _stub_record {
	uint	id;
	pvoid	proc;
	pvoid	stub;
	ulong	size;
} stub_record;

#define STUB_MOV_ESI_FLAGS				{0x90, 0x90, 0x90, 0x90, 0x90}
#define STUB_END_FLAGS					{0xCC, 0xCC, 0x19, 0x90, 0x04, 0x03, 0xCC, 0xCC}

#define STUB_ID_GET_MODULE_HANDLE_A		0x00
#define STUB_ID_GET_MODULE_HANDLE_W		0x01
#define STUB_ID_GET_MODULE_FILE_NAME_A	0x02
#define STUB_ID_GET_MODULE_FILE_NAME_W	0x03
#define STUB_ID_GET_PROC_ADDRESS		0x04

#define stub_GetModuleHandleA			stub_addrof(GetModuleHandleAStub)
#define stub_GetModuleHandleW			stub_addrof(GetModuleHandleWStub)
#define stub_GetModuleFileNameA			stub_addrof(GetModuleFileNameAStub)
#define stub_GetModuleFileNameW			stub_addrof(GetModuleFileNameWStub)
#define stub_GetProcAddress				stub_addrof(GetProcAddressStub)

/************************************************************************/
/* 获取过程真实地址	                                                    */
/************************************************************************/
__inline pvoid stub_addrof(__in pvoid proc)
{
	asm_jmp *p = (asm_jmp*)proc;
	if(p->opcode == 0xE9) {
		return stub_addrof((pvoid)((puchar)proc + sizeof(asm_jmp) + p->operand));
	}
	return proc;
}

/************************************************************************/
/* 获取过程占用内存长度                                                 */
/************************************************************************/
__inline ulong stub_sizeof(__in pvoid proc)
{
	uint i;
	puchar p = (puchar)proc;
	uchar  end_flags[] = STUB_END_FLAGS;

	for(i = 0; i < PAGE_SIZE; p += 1, i++) {
		if(memcmp(p, & end_flags[0], sizeof(end_flags)) == 0) {
			break;
		}
	}

	return i;
}

/************************************************************************/
/* 设置Mov Esi, 0x********                                              */
/************************************************************************/
__inline void stub_mov_esi(__in pvoid proc, __in image_information *info)
{
	uint i;
	puchar p = (puchar)proc;
	uchar  end_flags[]  = STUB_MOV_ESI_FLAGS;
	asm_mov_esi mov_esi = {0xBE, (ulong)(_W64 ulong)info};

	for(i = 0; i < PAGE_SIZE; p += 1, i++) {
		if(memcmp(p, & end_flags[0], sizeof(end_flags)) == 0) {
			break;
		}
	}

	*(asm_mov_esi*)p = mov_esi;
}

/************************************************************************/
/* 获取中间过程列表                                                     */
/************************************************************************/
__inline stub_record* stubs()
{
#ifndef STUB_RECORD
#define STUB_RECORD(id, p) {id, p, stub_ ## p, stub_sizeof(stub_ ## p)},
#endif
#ifndef STUB_RECORD_END
#define STUB_RECORD_END	   {0, 0, 0, 0}
#endif
	//////////////////////////////////////////////////////////////////////////
	// records
	static stub_record records[] = {
		STUB_RECORD(STUB_ID_GET_MODULE_HANDLE_A,	GetModuleHandleA)
		STUB_RECORD(STUB_ID_GET_MODULE_HANDLE_W,	GetModuleHandleW)
		STUB_RECORD(STUB_ID_GET_MODULE_FILE_NAME_A, GetModuleFileNameA)
		STUB_RECORD(STUB_ID_GET_MODULE_FILE_NAME_W, GetModuleFileNameW)
		STUB_RECORD(STUB_ID_GET_PROC_ADDRESS,		GetProcAddress)

		STUB_RECORD_END
	};

	return & records[0];
}

#endif