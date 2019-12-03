/* __image_source_file__ */

#include "image.h"

#ifdef _WIN32
	#include "pe.h"
#elif defined(_LINUX)
	#include "elf.h"
#else
	#error Current platform is not supported
#endif

// Load image from buffer
PImageHeader ImageLoad(__IN PVOID Buffer, __IN USHORT Machine, __IN USHORT FileType)
{
	// Image header
	PImageHeader header  = NULL;
	ULONG	SizeOfHeader = 0,
			SizeOfImage  = 0;

	// Validate
	if (!ImageIsValid(Buffer, Machine, FileType)) {
		RETURN_FAIL(NULL, IMAGE_ERROR_INVALID_FILE);
	}

	// Header size
	SizeOfHeader = sizeof(ImageHeader) + ImageNumberOfHijack() * sizeof(ImageHijack);
	// Image size
	SizeOfImage = ImageSizeOfImage(Buffer);
	if (SizeOfImage < PAGE_SIZE) {
		RETURN_FAIL(NULL, IMAGE_ERROR_INVALID_FILE_SIZE);
	}

	// Alloc memory
	header = ImageAlloc(SizeOfHeader + SizeOfImage);

	header->MagicNumber		= IMAGE_HEADER_MAGIC_SIGNATURE;
	header->SizeOfHeader	= SizeOfHeader;
	header->SizeOfImage		= SizeOfImage;
	header->NumberOfHijack	= ImageNumberOfHijack();

	IMAGE_FIRST_HIJACK(header)->Buffer;

	RETURN_SUCCESS(header);

RETURN_FAIL_1:
	ImageFree(header);


	RETURN_FAIL(NULL, 0);
}

// Unload image
BOOL ImageUnload(__IN PImageHeader Header)
{
	if (Header->MagicNumber != IMAGE_HEADER_MAGIC_SIGNATURE) {
		RETURN_FAIL(FALSE, IMAGE_ERROR_INVALID_HEADER);
	}
	RETURN_SUCCESS(TRUE);
}