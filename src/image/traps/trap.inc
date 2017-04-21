; __trap_header__

IFNDEF 	 TRAP_INC
TRAP_INC Equ <1>

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

TRAP_ID_GET_MODULE_HANDLE_A		Equ <0>
TRAP_ID_GET_MODULE_HANDLE_W		Equ <1>
TRAP_ID_GET_MODULE_FILE_NAME_A	Equ <2>
TRAP_ID_GET_MODULE_FILE_NAME_W	Equ <3>
TRAP_ID_GET_PROC_ADDRESS		Equ <4>

ImageTrap Struct
	procedure DWord ?
	trap 	  DWord ?
	nSize	  DWord ?
ImageTrap EndS

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
	traps ImageTrap 20 Dup(<?, ?, ?>)

ImageInformation EndS

ENDIF
