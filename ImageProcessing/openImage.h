#pragma once
#include "tools.h"
#include "transformImage.h"
#include "display.h"

Image *openImage(const char *filename);
void saveSquare(Image *image, const char *filename, Point *point, int size);
void grayscaleImage(Image *image);
void saveImage(Image *image, const char *filename);
void saveBoard(Image *image, const char *filename);