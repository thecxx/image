/* __image_source_file__ */

#include "image.h"

#ifdef _WIN32
	#include "pe.h"
#elif defined(_LINUX)
	#include "elf.h"
#else
	#error Current platform is not supported
#endif


PImageHeader ImageLoad(__IN PVOID Buffer)
{
	// Validate
	if(!ImageIsValid(Buffer, IMAGE_FILE_MACHINE_SUPPORTED, IMAGE_FILE_DYNAMIC_LIBRARY)) {
		return NULL;
	}
	return NULL;
}

BOOL ImageUnload(__IN PImageHeader Header)
{
	return TRUE;
}