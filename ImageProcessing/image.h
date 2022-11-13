#pragma once

#include "tools.h"
#include "quad.h"

typedef struct
{
	uc nb_channels; // 1: G, (3: RGB,) 4: RGBA
	st width;
	st height;
	uc **channels; // uc channels[nb_channels][width*hright]
} Image;

uc *copyChannel(uc *pixels, st len);
Image *newImage(uc nb_channels, st width, st height);
Image *copyImage(Image *image);
void freeImage(Image *image);
Image *openImage(const char *filename, uc nb_channels);
void placeDigit(Image *background, Image *digit, Quad *grid, int i, int j);
