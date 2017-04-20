;EasyCodeName=GetModuleFileNameWStub,1
.386
.Model flat, StdCall
Option CaseMap: none

Include	stub.inc

.Const

.Data?

.Data

.Code

GetModuleFileNameWStub Proc Uses Ebx Ecx Edx Esi Edi, hModule:DWord, lpFilename: Ptr WCHAR, nSize:DWord
	; Mov Esi, 0x********
	Mov_Esi_Information
	;// Pointer to ImageInformation
	Assume Esi: Ptr ImageInformation

	Mov Eax, [Esi].imagebase

	.If hModule == Eax
		Mov Ebx, Esi
		Assume Ebx: Ptr ImageInformation

		Lea Esi, [Ebx].ModulePathW
		Mov Edi, lpFilename
		Mov Ecx, nSize
		Cld
		Rep Movsw

		Mov Eax, [Ebx].LengthOfPath
	.Else
		Push nSize
		Push lpFilename
		Push hModule
		Call [Esi].stubs[SizeOf ImageStub * STUB_ID_GET_MODULE_FILE_NAME_W].procedure
	.EndIf

	Return Eax
GetModuleFileNameWStub EndP

End
