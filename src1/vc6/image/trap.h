/* __trap_header__ */

#ifndef __TRAP_H__
#define __TRAP_H__

#include "image.h"

#ifdef __cplusplus
extern "C"
{
#endif

HMODULE __syscall GetModuleHandleATrap(__in __opt LPCSTR lpModuleName);
HMODULE __syscall GetModuleHandleWTrap(__in __opt LPCWSTR lpModuleName);
DWORD   __syscall GetModuleFileNameATrap(__in __opt HMODULE hModule, __out LPSTR lpFilename, __in DWORD nSize);
DWORD   __syscall GetModuleFileNameWTrap(__in __opt HMODULE hModule, __out LPWSTR lpFilename, __in DWORD nSize);
FARPROC __syscall GetProcAddressTrap(__in HMODULE hModule, __in LPCSTR lpProcName);

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

typedef struct _trap_record {
	uint	id;
	pvoid	proc;
	pvoid	trap;
	ulong	size;
} trap_record;

#define TRAP_MOV_ESI_FLAGS				{0x90, 0x90, 0x90, 0x90, 0x90}
#define TRAP_END_FLAGS					{0xCC, 0xCC, 0x19, 0x90, 0x04, 0x03, 0xCC, 0xCC}

#define TRAP_ID_GET_MODULE_HANDLE_A		0x00
#define TRAP_ID_GET_MODULE_HANDLE_W		0x01
#define TRAP_ID_GET_MODULE_FILE_NAME_A	0x02
#define TRAP_ID_GET_MODULE_FILE_NAME_W	0x03
#define TRAP_ID_GET_PROC_ADDRESS		0x04

#define trap_GetModuleHandleA			trap_addrof(GetModuleHandleATrap)
#define trap_GetModuleHandleW			trap_addrof(GetModuleHandleWTrap)
#define trap_GetModuleFileNameA			trap_addrof(GetModuleFileNameATrap)
#define trap_GetModuleFileNameW			trap_addrof(GetModuleFileNameWTrap)
#define trap_GetProcAddress				trap_addrof(GetProcAddressTrap)

/************************************************************************/
/* 获取过程真实地址	                                                    */
/************************************************************************/
__inline pvoid trap_addrof(__in pvoid proc)
{
	asm_jmp *p = (asm_jmp*)proc;
	if(p->opcode == 0xE9) {
		return trap_addrof((pvoid)((puchar)proc + sizeof(asm_jmp) + p->operand));
	}
	return proc;
}

/************************************************************************/
/* 获取过程占用内存长度                                                 */
/************************************************************************/
__inline ulong trap_sizeof(__in pvoid proc)
{
	uint i;
	puchar p = (puchar)proc;
	uchar  end_flags[] = TRAP_END_FLAGS;

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
__inline void trap_mov_esi(__in pvoid proc, __in image_information *info)
{
	uint i;
	puchar p = (puchar)proc;
	uchar  end_flags[]  = TRAP_MOV_ESI_FLAGS;
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
__inline trap_record* traps()
{
#ifndef TRAP_RECORD
#define TRAP_RECORD(id, p) {id, p, trap_ ## p, trap_sizeof(trap_ ## p)},
#endif
#ifndef TRAP_RECORD_END
#define TRAP_RECORD_END	   {0, 0, 0, 0}
#endif
	//////////////////////////////////////////////////////////////////////////
	// records
	static trap_record records[] = {
		TRAP_RECORD(TRAP_ID_GET_MODULE_HANDLE_A,	GetModuleHandleA)
		TRAP_RECORD(TRAP_ID_GET_MODULE_HANDLE_W,	GetModuleHandleW)
		TRAP_RECORD(TRAP_ID_GET_MODULE_FILE_NAME_A, GetModuleFileNameA)
		TRAP_RECORD(TRAP_ID_GET_MODULE_FILE_NAME_W, GetModuleFileNameW)
		TRAP_RECORD(TRAP_ID_GET_PROC_ADDRESS,		GetProcAddress)

		TRAP_RECORD_END
	};

	return & records[0];
}

#endif