#pragma once

#include "image.h"
#include "imageRGBA.h" // new
#include "matrices.h"
#include <math.h>
#include <stdio.h>
#include <err.h>

void resizeImage(Image *image, st new_w, st new_h);
void resizeImageRGBA(ImageRGBA *image, st new_w, st new_h);
void autoResize(Image *image, st maw_w, st max_h);
Image* rotateImage(Image * image, int angle, uc background_color);
void integrateNumber(Image *image, Image *number, Point *origin);
Image *extractGrid(Image *image, Quad *quad, st new_w, st new_h);
Quad *rotateQuad(Quad *quad, int theta, Image *image, Image *rotated);
