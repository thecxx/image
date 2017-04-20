; __stub_header__

IFNDEF 	 STUB_INC
STUB_INC Equ <1>

CHAR 	TypeDef Byte
WCHAR 	TypeDef Word

Mov_Esi_Information Macro
	Nop
	Nop
	Nop
	Nop
	Nop
EndM

End_Flags Macro
	DB 0CCH, 0CCH, 019H, 090H, 004H, 003H, 0CCH, 0CCH
EndM

Return Macro x: = < Eax >
	Mov Eax, x
	Ret
	End_Flags
EndM

STUB_ID_GET_MODULE_HANDLE_A		Equ <0>
STUB_ID_GET_MODULE_HANDLE_W		Equ <1>
STUB_ID_GET_MODULE_FILE_NAME_A	Equ <2>
STUB_ID_GET_MODULE_FILE_NAME_W	Equ <3>
STUB_ID_GET_PROC_ADDRESS		Equ <4>

ImageStub Struct
	procedure DWord ?
	stub 	  DWord ?
	nSize	  DWord ?
ImageStub EndS

ImageApis Struct
	MessageBoxA DWord Ptr ?
	MessageBoxW DWord Ptr ?
    lstrcmpiA   DWord Ptr ?
    lstrcmpiW   DWord Ptr ?
ImageApis EndS

ImageInformation Struct

	nSize 	  DWord ?

	imagebase DWord ?
	imagesize DWord ?

	ModuleBaseNameA CHAR 	32  Dup(?)
	ModuleBaseNameW WCHAR 	32  Dup(?)
	ModuleNameA CHAR 	    32  Dup(?)
	ModuleNameW WCHAR 	    32  Dup(?)
	ModulePathA CHAR 	    260 Dup(?)
	ModulePathW WCHAR 	    260 Dup(?)

	LengthOfName DWord ?
	LengthOfPath DWord ?

	apis  ImageApis <?, ?, ?, ?>
	stubs ImageStub 20 Dup(<?, ?, ?>)

ImageInformation EndS

ENDIF
