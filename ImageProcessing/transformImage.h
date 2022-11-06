#pragma once

#include <err.h>
#include "tools.h"

void invertImage(Image *image);
void medianFilter(Image *image, int radius);
void gaussianBlur(Image *image);
void calibrateImage(Image *image, int radius, uc default_value);
void sobelFilter(Image *image);
void saturateImage(Image *image);
void resizeImage(Image *image, st new_w, st new_h);
void autoResize(Image *image, st maw_w, st max_h);
Image* rotateImage(Image * image, int angleD, uc background_color);
void integrateNumber(Image *image, Image *number, Point *origin);
Image *extractGrid(Image *image, Quadri *quadri, st new_w, st new_h);