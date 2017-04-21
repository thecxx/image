;EasyCodeName=GetProcAddress,1
.386
.Model flat, StdCall
Option CaseMap: none

Include	trap.inc

.Const

.Data?

.Data

.Code

GetProcAddressTrap Proc Uses Ebx Ecx Edx Edi Esi hModule:DWord, lpProcName: Ptr CHAR
	; Mov Esi, 0x********
	Mov_Esi_Information
	; Pointer to ImageInformation
	Assume Esi: Ptr ImageInformation

	Mov Eax, [Esi].traps[SizeOf ImageTrap * TRAP_ID_GET_PROC_ADDRESS].procedure
	Push lpProcName
	Push hModule
	Call Eax

	.If Eax != 0
		Xor Ecx, Ecx

		.While Ecx < 20

			Push Eax

			Lea Ebx, [Esi].traps[0]
			Mov Eax, SizeOf ImageTrap
			Mul Ecx
			Add Ebx, Eax

			Pop Eax

			Assume Ebx: Ptr ImageTrap

			.If [Ebx].procedure == 0
				.Break
			.EndIf

			.If [Ebx].procedure == Eax
				Mov Eax, [Ebx].trap
				.Break
			.EndIf

			Inc Ecx
		.EndW
	.EndIf

	Return Eax
GetProcAddressTrap EndP

End