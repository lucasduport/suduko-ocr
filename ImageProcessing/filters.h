#pragma once

#include "image.h"

void invertImage(Image *image);
void thresholdCells(Image *image);
void gaussianBlur(Image *image);
void calibrateImage(Image *image, int radius);
void sobelFilter(Image *image);
void saturateImage(Image *image);
