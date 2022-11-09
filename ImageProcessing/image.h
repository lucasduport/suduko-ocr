#pragma once

#include "tools.h"

typedef struct
{
	uc *pixels;
	st width;
	st height;
} Image;

uc *copyPixels(uc *pixels, st len);
Image *newImage(st width, st height);
Image *copyImage(Image *image);
void freeImage(Image *image);
Image *openImage(const char *filename);
