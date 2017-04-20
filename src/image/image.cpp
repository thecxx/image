/* __image_source__ */

#include "image.h"
#include "mm.h"
#include "pe.h"
#include "stub.h"

bool image_init(__out image_information *info,
				__in pvoid buffer,
				__in ulong size,
				__in malloc_t m,
				__in free_t f)
{
	// Fill zero
	RtlZeroMemory(info, sizeof(image_information));

	// 文件格式检查
	if(!pe_is(buffer) || !pe_is_dll(buffer)) {
		return false;
	}
	
	info->imagebase = null;
	info->imagesize = 0;

	info->in.buffer = buffer;
	info->in.size	= size;

	info->mm.malloc = m;
	info->mm.free	= f;

	if(!image_init_apis(info)) {
		return false;
	}

	if(!image_init_stubs(info)) {
		return false;
	}

	return true;
}

bool image_init_apis(__in image_information *info)
{
#define DECLARE_API(p)	(info->apis. ## p ## = p);
	DECLARE_API(MessageBoxA);
	DECLARE_API(MessageBoxW);
	DECLARE_API(lstrcmpiA);
	DECLARE_API(lstrcmpiW);
#undef  DECLARE_API

	return true;
}

bool image_init_stubs(__in image_information *info)
{
	uint id;
	stub_record *record = stubs();

	while(record->proc) {
		id = record->id;
		if(id >= IMAGE_STUB_MAX) {
			return false;
		}

		info->stubs[id].proc = record->proc;
		info->stubs[id].stub = record->stub;
		info->stubs[id].size = record->size;

		record += 1;
	}

	return true;
}

pvoid image_load(__in image_information *info)
{
	uint  i;
	DWORD time;
	dllmain_t dllmain;

	for(i = 0; i < IMAGE_STUB_MAX; i++) {
		if(info->stubs[i].proc == null) {
			break;
		}
	}

	// 计算需要内存大小
	info->imagesize = image_sizeof(info->in.buffer);
	info->size		= (sizeof(image_information) - sizeof(image_stub_text)) + (sizeof(image_stub_text) * i);

	// alloc memory
	info->imagebase = info->mm.malloc(info->imagesize + info->size);
	if(info->imagebase == null) {
		goto __RETURN_NULL;
	}

	// Fill zero
	RtlZeroMemory(info->imagebase, info->imagesize);
	// Init privilege
	mm_protect(info->imagebase, info->imagesize + info->size, PAGE_EXECUTE_READWRITE, null);

	//////////////////////////////////////////////////////////////////////////
	// copy information
	*(image_information*)((puchar)info->imagebase + info->imagesize) = *info;
	info = (image_information*)((puchar)info->imagebase + info->imagesize);

#ifdef _DEBUG
	time = 11111111;
#else
	time = GetCurrentTime();
#endif

	sprintf_s	<32>	(info->modulebasename.A,	"%d",					  time);
	swprintf_s	<32>	(info->modulebasename.W,	_T("%d"),				  time);
	sprintf_s	<32>	(info->modulename.A,		"%d.dll",				  time);
	swprintf_s	<32>	(info->modulename.W,		_T("%d.dll"),			  time);
	sprintf_s	<260>	(info->modulepath.A,		"C:\\fakepath\\%s",		& info->modulename.A[0]);
	swprintf_s	<260>	(info->modulepath.W,		_T("C:\\fakepath\\%s"),	& info->modulename.W[0]);

	info->LengthOfName = (ulong)strlen(& info->modulename.A[0]);
	info->LengthOfPath = (ulong)strlen(& info->modulepath.A[0]);

	// copy stubs
	image_copy_stubs(info);

	//////////////////////////////////////////////////////////////////////////
	// copy image
	image_copy_im(info);

	// relocation
	if(!image_relocation(info->imagebase, info)) {
		goto __RETURN_FREE;
	}
	// fix import table
	if(!image_fix_import(info->imagebase, info)) {
		goto __RETURN_FREE;
	}

	pe_optional_header(info->imagebase)->ImageBase	= (ulong)(_W64 ulong)info->imagebase;
	pe_optional_header(info->imagebase)->SizeOfImage= info->imagesize;

	__try {
		dllmain = pe_entry<dllmain_t>(info->imagebase);	
		if(!dllmain || !dllmain(info->imagebase, DLL_PROCESS_ATTACH, 0)) {
			goto __RETURN_FREE;
		}
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		goto __RETURN_FREE;
	}

	return info->imagebase;

__RETURN_FREE:
	image_free(info->imagebase);

__RETURN_NULL:

	return null;
}

pvoid image_copy_im(__in image_information *info)
{
	uint i;
	pvoid addr		= info->imagebase;
	pvoid buffer	= info->in.buffer;
	PIMAGE_SECTION_HEADER  SectionHeader;

	// copy header
	RtlMoveMemory(addr, buffer, pe_dos_header(buffer)->e_lfanew + sizeof(IMAGE_NT_HEADERS) + PE_SECTION_COUNT(buffer) * sizeof(IMAGE_SECTION_HEADER));

	// copy sections
	for(i = 0; i < PE_SECTION_COUNT(buffer); i++) {
		SectionHeader = PE_SECTION(buffer, i);
		if(SectionHeader->VirtualAddress) {
			RtlMoveMemory(((puchar)addr + SectionHeader->VirtualAddress), ((puchar)buffer + SectionHeader->PointerToRawData), SectionHeader->SizeOfRawData);
		}
	}

	return addr;

}

void image_copy_stubs(__in image_information *info)
{
	uint i;
	for(i = 0; i < IMAGE_STUB_MAX; i++) {
		if(info->stubs[i].proc == null) {
			break;
		}
		RtlMoveMemory(& info->text[i], info->stubs[i].stub, info->stubs[i].size);
		// Update stub pointer
		info->stubs[i].stub = (pvoid)& info->text[i];
		// Set information
		stub_mov_esi(info->stubs[i].stub, info);
	}
}


bool image_relocation(__in pvoid addr, __in image_information *info)
{
	ulong delta;
	ulong index = 0;

	pulong address = null;
	ulong  type = 0;
	ulong  offset = 0;

	reloc_block *block = null;
	ulong  number = 0;

	PIMAGE_OPTIONAL_HEADER OptionalHeader = pe_optional_header(addr);
	PIMAGE_BASE_RELOCATION Location		  = pe_data_directory<PIMAGE_BASE_RELOCATION>(addr, IMAGE_DIRECTORY_ENTRY_BASERELOC, null);

	if(OptionalHeader == null) {
		return false;
	} else if(Location != null) {

		// 修正值
		delta = (ulong)(_W64 ulong)((puchar)addr - OptionalHeader->ImageBase);

		while(Location->VirtualAddress != 0)
		{
			// 遍历重定位表
			block  = (reloc_block*)((puchar)Location + sizeof(IMAGE_BASE_RELOCATION));
			number = (Location->SizeOfBlock          - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(reloc_block);

			for(index = 0; index < number; index++) {

				type   = (block[index] & 0xF000) >> 12;
				offset = (block[index] & 0x0FFF);

				address = (pulong)((puchar)addr + Location->VirtualAddress + offset);

				switch(type)
				{
					case IMAGE_REL_BASED_HIGHLOW:
						*address += delta;
						break;

					default:
						break;
				}			
			}

			// 下一张重定位表
			Location = (PIMAGE_BASE_RELOCATION)((puchar)Location + Location->SizeOfBlock);
		}		
	}

	return true;
}

bool image_fix_import(__in pvoid addr, __in image_information *info)
{
	uint i;
	ulong index = 0;

	LPCSTR ModuleName;
	pvoid  ModuleHandle = null;
	LPCSTR FunctionName = null;
	pvoid  FunctionAddr = null;

	PIMAGE_THUNK_DATA OriginTable;
	PIMAGE_THUNK_DATA FuncTable;

	PIMAGE_IMPORT_DESCRIPTOR ImportTable = pe_data_directory<PIMAGE_IMPORT_DESCRIPTOR>(addr, IMAGE_DIRECTORY_ENTRY_IMPORT, null);
	if(ImportTable == null) {
		return false;
	}

	// 循环修复
	while(ImportTable->FirstThunk && ImportTable->OriginalFirstThunk)
	{
		OriginTable = (PIMAGE_THUNK_DATA)((puchar)addr + ImportTable->OriginalFirstThunk);
		FuncTable   = (PIMAGE_THUNK_DATA)((puchar)addr + ImportTable->FirstThunk);

		ModuleName = (LPCSTR)((puchar)addr + ImportTable->Name);

		if((ModuleHandle = GetModuleHandleA(ModuleName)) || (ModuleHandle = LoadLibraryA(ModuleName))) {

			// 查找到需要的模块
			for(index = 0; OriginTable[index].u1.Ordinal != 0; index++) {

				// 函数地址
				FunctionAddr = null;
				FunctionName = null;

				if(OriginTable[index].u1.Ordinal & IMAGE_ORDINAL_FLAG) {
					FunctionName = null;
					FunctionAddr = image_proc(ModuleHandle, (LPCSTR)(_W64 ulong)(OriginTable[index].u1.Ordinal & 0x0000FFFF));
				} else {
					FunctionName = (LPCSTR) &(((PIMAGE_IMPORT_BY_NAME)((puchar)addr + OriginTable[index].u1.AddressOfData))->Name[0]);
					FunctionAddr = image_proc(ModuleHandle, FunctionName);
				}

				if(FunctionAddr) {
					for(i = 0; i < IMAGE_STUB_MAX; i++) {
						if(info->stubs[i].proc == null) {
							break;
						} else if(info->stubs[i].proc == FunctionAddr) {
							FunctionAddr = info->stubs[i].stub;
							break;
						}
					}
					FuncTable[index].u1.Function = (ulong)(_W64 ulong)FunctionAddr;
				} else {
					return false;
				}

			}

			// 
			image_fix_import_for(ModuleHandle, info);

		} else {
			return false;
		}

		// 下一张表
		ImportTable = (PIMAGE_IMPORT_DESCRIPTOR)((puchar)ImportTable + sizeof(IMAGE_IMPORT_DESCRIPTOR));
	}

	return true;
}

bool image_fix_import_for(__in pvoid hModule, __in image_information *info)
{
	uint i;
	ulong index = 0;

	LPCSTR  ModuleName;
	LPCSTR  FunctionName;
	pvoid	FunctionAddr;
	DWORD   OldProtect;

	PIMAGE_THUNK_DATA OriginTable;
	PIMAGE_THUNK_DATA FuncTable;

	PIMAGE_IMPORT_DESCRIPTOR ImportTable = pe_data_directory<PIMAGE_IMPORT_DESCRIPTOR>(hModule, IMAGE_DIRECTORY_ENTRY_IMPORT, null);
	if(ImportTable == null) {
		return false;
	}

	// 循环修复
	while(ImportTable->FirstThunk && ImportTable->OriginalFirstThunk)
	{
		OriginTable = (PIMAGE_THUNK_DATA)((puchar)hModule + ImportTable->OriginalFirstThunk);
		FuncTable   = (PIMAGE_THUNK_DATA)((puchar)hModule + ImportTable->FirstThunk);

		ModuleName = (LPCSTR)((puchar)hModule + ImportTable->Name);
		// 查找到需要的模块
		for(index = 0; OriginTable[index].u1.Ordinal != 0; index++) {

			// 函数地址
			FunctionAddr = (pvoid)(pulong)(_W64 ulong)FuncTable[index].u1.Function;
			FunctionName = null;

			if(OriginTable[index].u1.Ordinal & IMAGE_ORDINAL_FLAG) {
				FunctionName = null;
			} else {
				FunctionName = (LPCSTR) &(((PIMAGE_IMPORT_BY_NAME)((puchar)hModule + OriginTable[index].u1.AddressOfData))->Name[0]);
			}

			for(i = 0; i < IMAGE_STUB_MAX; i++) {
				if(info->stubs[i].proc == null) {
					break;
				} else if(info->stubs[i].proc == FunctionAddr) {
					mm_protect(& FuncTable[index].u1.Function, sizeof(pvoid), PAGE_EXECUTE_READWRITE, & OldProtect);
					FuncTable[index].u1.Function = (ulong)(_W64 ulong)info->stubs[i].stub;
					mm_protect(& FuncTable[index].u1.Function, sizeof(pvoid), OldProtect, null);

					image_hook_push(info, (pvoid*)& FuncTable[index].u1.Function, FunctionAddr);

					break;
				}
			}
		}

		// 下一张表
		ImportTable = (PIMAGE_IMPORT_DESCRIPTOR)((puchar)ImportTable + sizeof(IMAGE_IMPORT_DESCRIPTOR));
	}

	return true;
}

ulong image_sizeof(__in pvoid addr)
{
	uint i;
	uint size;
	uint alignment;
	uint SizeOfSection;
	PIMAGE_SECTION_HEADER  SectionHeader;
	PIMAGE_OPTIONAL_HEADER OptionHeader;

	OptionHeader = pe_optional_header(addr);
	if(OptionHeader == null) {
		return 0;
	}

	size	  = OptionHeader->SizeOfImage;
	alignment = OptionHeader->SectionAlignment;

	// alignment
	size = image_alignment(size, alignment);

	for(i = 0; i < PE_SECTION_COUNT(addr); i++) {
		SectionHeader = PE_SECTION(addr, i);

		SizeOfSection = max(SectionHeader->SizeOfRawData, SectionHeader->Misc.VirtualSize);
		size		  = max(size, SectionHeader->VirtualAddress + SizeOfSection);
		// alignment
		size = image_alignment(size, alignment);
	}

	return size;

}

pvoid image_load(__in pvoid buffer, __in ulong size, __in malloc_t m /* = null */, __in free_t f /* = null */)
{
	image_information info = {0};

	if(!image_init(&info, buffer, size, (m ? m : mm_malloc), (f ? f : mm_free))) {
		return null;
	}
	return image_load(&info);
}

void image_free(__in pvoid addr)
{
	DWORD OldProtect;
	image_hook hook = {0};
	image_information *info = TO_INFO(addr);

	dllmain_t dllmain = pe_entry<dllmain_t>(addr);

	if(dllmain) {
		dllmain(addr, DLL_PROCESS_DETACH, 0);
	}

	while(image_hook_pop(info, &hook)) {
		// free hook
		mm_protect(hook.address, sizeof(pvoid), PAGE_EXECUTE_READWRITE, & OldProtect);
		*hook.address = hook.function;
		mm_protect(hook.address, sizeof(pvoid), OldProtect, null);
	}

	info->mm.free(addr);
}

pvoid image_proc(__in pvoid addr, __in const char *name)
{
	pulong  FunctionNames;
	pulong  FunctionAddrs;
	pushort FunctionOrds;

	ulong   Index;
	LPSTR   FunctionName;
	pvoid FunctionAddr = null;

	// 重定向
	char ForwardModuleName[128] = {0};
	char ForwardProcName[128] = {0};

	LPCSTR ForwardString = null;
	ulong  Length;
	pvoid  hForwardModule;

	PIMAGE_EXPORT_DIRECTORY ExportTable;
	DWORD ExportTableSize;

	ExportTable     = pe_data_directory<PIMAGE_EXPORT_DIRECTORY>(addr, IMAGE_DIRECTORY_ENTRY_EXPORT, &ExportTableSize);
	if(ExportTable->NumberOfFunctions == 0) {
		return null;
	}

	FunctionAddr	= null;
	FunctionAddrs	= (pulong)((puchar)addr + ExportTable->AddressOfFunctions);

	if((ulong)(_W64 ulong)name & 0xFFFF0000) {
		if(ExportTable->NumberOfNames > 0 && ExportTable->AddressOfNames > 0 && ExportTable->AddressOfNameOrdinals > 0)
		{
			FunctionNames	= (pulong)((puchar)addr + ExportTable->AddressOfNames);	
			FunctionOrds	= (pushort)((puchar)addr + ExportTable->AddressOfNameOrdinals);

			for(Index = 0; Index < ExportTable->NumberOfNames; Index++)
			{
				// 函数名
				FunctionName = (LPSTR)((puchar)addr + FunctionNames[Index]);
				if(strcmp(FunctionName, name) == 0) {
					FunctionAddr = (pvoid)((puchar)addr + FunctionAddrs[FunctionOrds[Index]]);
					break;
				}
			}
		}
	} else {
		// Index = OrdinalNumber - Base
		Index = ((ulong)(_W64 ulong)name & 0x0000FFFF) - ExportTable->Base;

		if(ExportTable->AddressOfNameOrdinals > 0)
		{
			FunctionOrds = (pushort)((puchar)addr + ExportTable->AddressOfNameOrdinals);
			FunctionAddr = (pvoid)((puchar)addr + FunctionAddrs[FunctionOrds[Index]]);
		}
		else if(Index < ExportTable->NumberOfFunctions)
		{
			FunctionAddr = (pvoid)((puchar)addr + FunctionAddrs[Index]);
		}

	}

	if(FunctionAddr && 
		((ulong)(_W64 ulong)FunctionAddr) >= ((ulong)(_W64 ulong)ExportTable) && 
		((ulong)(_W64 ulong)FunctionAddr) <= (((ulong)(_W64 ulong)ExportTable) + ExportTableSize)) {
			// 重定向字符串
			ForwardString = (char*)FunctionAddr;

			// 提取模块名
			Length = (ulong)(_W64 ulong)(strchr(ForwardString, '.') - ForwardString);
			RtlMoveMemory(ForwardModuleName, ForwardString, Length);

			ForwardString += Length + 1;

			// 提取接口名
			Length = (ulong)(_W64 ulong)(strchr(ForwardString, '\0') - ForwardString);
			RtlMoveMemory(ForwardProcName, ForwardString, Length);

			if(hForwardModule = GetModuleHandleA(ForwardModuleName)) {
				FunctionAddr = image_proc(hForwardModule, ForwardProcName);
			} else {
				FunctionAddr = null;
			}

	}

	return FunctionAddr;
}

pvoid image_to_im(__in image_information *info)
{
	return info->imagebase;
}

image_information* image_to_information(__in pvoid addr)
{
	return (image_information*)((puchar)addr + pe_optional_header(addr)->SizeOfImage);
}

image_hook* image_hook_push(__in image_information *info, __in pvoid *address, __in pvoid function)
{
	image_hook *hook = (image_hook*)info->mm.malloc(sizeof(image_hook));

	hook->head.Blink = null;
	hook->head.Flink = null;
	hook->address	 = address;
	hook->function	 = function;

	if(info->hook) {
		hook->head.Flink = & info->hook->head;
	}

	return info->hook = hook;
}

bool image_hook_pop(__in image_information *info, __out image_hook *hook)
{
	image_hook *p = info->hook;

	if(p == null) {
		return false;
	}
	if(hook) {
		*hook = *p;
		hook->head.Blink = null;
		hook->head.Flink = null;
	}

	info->hook = (image_hook*)p->head.Flink;
	info->mm.free(p);

	return true;
}

PVOID LdrLoadImage(__in PVOID Buffer, __in DWORD Size, __in malloc_t m /* = NULL */, __in free_t f /* = NULL */)
{
	return image_load(Buffer, Size, m, f);
}

PVOID LdrGetProcAddress(__in PVOID Addr, __in LPCSTR Name)
{
	return image_proc(Addr, Name);
}

VOID LdrFreeImage(__in PVOID Addr)
{
	image_free(Addr);
}