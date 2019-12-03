/* __pe_source_file__ */

#include "pe.h"

PIMAGE_DOS_HEADER PeDosHeader(__IN PVOID Addr)
{
	return (PIMAGE_DOS_HEADER)Addr;
}

PIMAGE_NT_HEADERS PeNtHeader(__IN PVOID Addr)
{
	return (PIMAGE_NT_HEADERS)((PUCHAR)Addr + PeDosHeader(Addr)->e_lfanew);
}

PIMAGE_FILE_HEADER PeFileHeader(__IN PVOID Addr)
{
	return & PeNtHeader(Addr)->FileHeader;
}

PIMAGE_OPTIONAL_HEADER PeOptionalHeader(__IN PVOID Addr)
{
	return & PeNtHeader(Addr)->OptionalHeader;
}

PIMAGE_SECTION_HEADER PeSectionHeader(__IN PVOID Addr, __IN UINT Index)
{
	return &IMAGE_FIRST_SECTION(PeNtHeader(Addr))[Index];
}

BOOL PeIsValid(__IN PVOID Addr, __IN USHORT Machine, __IN USHORT Characteristics)
{
	if (PeDosHeader(Addr)->e_magic != IMAGE_DOS_SIGNATURE || PeNtHeader(Addr)->Signature != IMAGE_NT_SIGNATURE) {
		return FALSE;
	}
	if ((PeFileHeader(Addr)->Machine & Machine) != Machine) {
		return FALSE;
	}
	if ((PeFileHeader(Addr)->Characteristics & Characteristics) != Characteristics) {
		return FALSE;
	}
	return TRUE;
}

ULONG PeSectionNumber(__IN PVOID Addr)
{
	return PeFileHeader(Addr)->NumberOfSections;
}

ULONG PeSizeOfImage(__IN PVOID Addr)
{
	UINT i;
	ULONG size;
	ULONG alignment;
	ULONG SizeOfSection;
	PIMAGE_SECTION_HEADER  SectionHeader;
	PIMAGE_OPTIONAL_HEADER OptionHeader;

	OptionHeader = PeOptionalHeader(Addr);
	if (OptionHeader == NULL) {
		return 0;
	}

	size	  = OptionHeader->SizeOfImage;
	alignment = OptionHeader->SectionAlignment;

	// Alignment
	size = PeAlignment(size, alignment);

	for (i = 0; i < PeSectionNumber(Addr); i++) {
		SectionHeader = PeSectionHeader(Addr, i);

		SizeOfSection = max(SectionHeader->SizeOfRawData, SectionHeader->Misc.VirtualSize);
		size		  = max(size, SectionHeader->VirtualAddress + SizeOfSection);
		// Alignment
		size = PeAlignment(size, alignment);
	}

	return size;
}

ULONG PeAlignment(__IN ULONG Size, __IN ULONG Alignment)
{
	return (Size % Alignment ? (((Size / Alignment) + 1) * Alignment) : Size);
}

ULONG PeNumberOfHijack()
{
	return 0;
}

BOOL PeInitHijack(__IN PImageHijack hijack, __IN UINT Index)
{
	return TRUE;
}

PVOID PeAlloc(__IN Size)
{
	return NULL;
}

VOID PeFree(__IN PVOID Ptr)
{

}