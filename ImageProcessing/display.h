#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "openImage.h"
#include "tools.h"

void showLines(Image *background, Segment **segments, st nb_segments,
int r, int g, int b, float thickness);
void showQuadri(Image *background, Quadri *quadri, int r, int g, int b);
SDL_Surface* imageToSurface(Image *image) ;
int event_loop(SDL_Renderer* renderer, Image *image);
int rotateWithView(Image *image);
void draw(SDL_Renderer* renderer, SDL_Texture * texture);
int displayImage(Image *image, char *windowName);