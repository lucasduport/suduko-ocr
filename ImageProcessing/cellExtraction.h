#pragma once

#include "image.h"

void saveCells(Image *image, int cell_size, int border_size, const char *filename);
Image **loadCells(int **solved, char *dirname);
void cleanPath(char *filename, char *dest);
