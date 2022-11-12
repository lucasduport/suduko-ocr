#pragma once

#include "quad.h"
#include "image.h"

typedef struct
{
	uc r, g, b, a;
} Pixel;

typedef struct
{
	Pixel *pixels;
	st width;
	st height;
} ImageRGBA;

Pixel *copyPixelsRGBA(Pixel *pixels, st len);
ImageRGBA *newImageRGBA(st width, st height);
ImageRGBA *copyImageRGBA(ImageRGBA *image);
void freeImageRGBA(ImageRGBA *image);
ImageRGBA *openImageRGBA(const char *filename);
void placeDigit(Image *bg, ImageRGBA *digit, Quad *grid, int i, int j);
