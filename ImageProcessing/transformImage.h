#pragma once

#include <err.h>
#include "tools.h"

void invertImage(Image *image);
void calibrateImage(Image *image, int radius);
void saturateImage (Image *image);
Image *resizeImage(Image *image, st new_w, st new_h);
Image *autoResize(Image *image, st maw_w, st max_h);
Image* rotateImage(Image * image, int angleD, uc background_color);
void integrateNumber(Image *image, Image *number, Point *origin);
Image *extractGrid(Image *image, Quadri *quadri, st new_w, st new_h);