#pragma once
#include "tools.h"
#include "point.h"
#include "image.h"

void saveSquare(Image *image, const char *filename, Point *point, int size);
void grayscaleImage(Image *image);
void saveImage(Image *image, const char *filename);
void saveBoard(Image *image, const char *filename);
