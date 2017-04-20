;EasyCodeName=GetProcAddress,1
.386
.Model flat, StdCall
Option CaseMap: none

Include	stub.inc

.Const

.Data?

.Data

.Code

GetProcAddressStub Proc Uses Ebx Ecx Edx Edi Esi hModule:DWord, lpProcName: Ptr CHAR
	; Mov Esi, 0x********
	Mov_Esi_Information
	; Pointer to ImageInformation
	Assume Esi: Ptr ImageInformation

	Mov Eax, [Esi].stubs[SizeOf ImageStub * STUB_ID_GET_PROC_ADDRESS].procedure
	Push lpProcName
	Push hModule
	Call Eax

	.If Eax != 0
		Xor Ecx, Ecx

		.While Ecx < 20

			Push Eax

			Lea Ebx, [Esi].stubs[0]
			Mov Eax, SizeOf ImageStub
			Mul Ecx
			Add Ebx, Eax

			Pop Eax

			Assume Ebx: Ptr ImageStub

			.If [Ebx].procedure == 0
				.Break
			.EndIf

			.If [Ebx].procedure == Eax
				Mov Eax, [Ebx].stub
				.Break
			.EndIf

			Inc Ecx
		.EndW
	.EndIf

	Return Eax
GetProcAddressStub EndP

End