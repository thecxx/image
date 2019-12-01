/* __image_header_file__ */

#ifndef __IMAGE_H__
#define __IMAGE_H__

typedef struct _ImageHeader {

	// Magic number
	ULONG MagicNumber;

	ULONG SizeOfHeader;
	ULONG SizeOfImage;

	UCHAR Buffer[1];

} ImageHeader, *PImageHeader;

#endif