#include "display.h"
#include <stdio.h>

void showLines(Image *background, Segment **segments, st nb_segments, int r,
			   int g, int b, float thickness) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Window *window;
	window =
		SDL_CreateWindow("Segments visualizer", 0, 0, 1, 1, SDL_WINDOW_SHOWN);
	if (window == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Renderer *renderer =
		SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Surface *surface = imageToSurface(background);
	if (surface == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_SetWindowSize(window, surface->w, surface->h);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_FreeSurface(surface);
	/// IMAGE AND LINES PRINTER
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect rec_dest = {0, 0, w, h};
	SDL_Event event;
	SDL_WaitEvent(&event);
	float xscale = 0, yscale = 0;
	SDL_RenderGetScale(renderer, &xscale, &yscale);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, &rec_dest);
	SDL_SetRenderDrawColor(renderer, r, g, b, 0);
	SDL_RenderSetScale(renderer, thickness, thickness);
	for (st i = 0; i < nb_segments; i++) {
		SDL_RenderDrawLine(
			renderer, segments[i]->x1 / thickness, segments[i]->y1 / thickness,
			segments[i]->x2 / thickness, segments[i]->y2 / thickness);
	}
	SDL_RenderSetScale(renderer, xscale, yscale);
	SDL_RenderPresent(renderer);

	/// KEEP DISPLAY RESULT
	int keepDisplay = 1;
	while (keepDisplay) {
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_QUIT: keepDisplay = 0; break;
		}
	}
	/// DESTRUCTION
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void showQuadri(Image *background, Quadri *quadri, int r, int g, int b) {
	Point *p1 = quadri->p1;
	Point *p2 = quadri->p2;
	Point *p3 = quadri->p3;
	Point *p4 = quadri->p4;
	Segment s1 = {p1->x, p1->y, p3->x, p3->y, 0, 0, 0};
	Segment s2 = {p1->x, p1->y, p2->x, p2->y, 0, 0, 0};
	Segment s3 = {p3->x, p3->y, p4->x, p4->y, 0, 0, 0};
	Segment s4 = {p2->x, p2->y, p4->x, p4->y, 0, 0, 0};
	Segment *segments[] = {&s1, &s2, &s3, &s4};
	showLines(background, segments, 4, r, g, b, 4);
}

SDL_Surface *imageToSurface(Image *image) {
	st w = image->width;
	st h = image->height;
	uc *pixels = image->pixels;
	SDL_Surface *surface =
		SDL_CreateRGBSurfaceWithFormat(0, w, h, 8, SDL_PIXELFORMAT_RGB888);
	SDL_LockSurface(surface);
	for (st y = 0; y < h; y++) {
		for (st x = 0; x < w; x++) {
			unsigned int value = pixels[y * w + x];
			unsigned int *pixel = surface->pixels + y * surface->pitch
								  + x * surface->format->BytesPerPixel;
			*pixel = (value << 16) | (value << 8) | value;
		}
	}
	SDL_UnlockSurface(surface);
	if (surface == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	return surface;
}

int displayImage(Image *image, char *windowName) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Window *window;
	window = SDL_CreateWindow(windowName, 0, 0, 1, 1,
							  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (window == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Renderer *renderer =
		SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Surface *surface = imageToSurface(image);
	if (surface == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_SetWindowSize(window, surface->w, surface->h);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_FreeSurface(surface);

	/// KEEP DISPLAY RESULT
	int keepDisplay = 1;
	while (keepDisplay) {
		SDL_Event event;
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				draw(renderer, texture);
			break;
		case SDL_QUIT: keepDisplay = 0; break;
		}
	}
	/// KEEP DISPLAY RESULT

	// DESTRUCTION
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}

// catch all events in an endless loop
int event_loop(SDL_Renderer *renderer, Image *image) {
	SDL_Surface *surface = imageToSurface(image);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Event event;
	int angle = 0;
	int step = 5;
	Image *rotated = rotateImage(image, 0, 0);
	int running = 1;
	while (running) {
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_QUIT: running = 0; break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				draw(renderer, texture);
			break;
		case SDL_KEYDOWN:
			if (event.key.repeat != 0) break;
			if (event.key.keysym.scancode == SDL_SCANCODE_S)
				saveImage(rotated, "rotated.png");
			if (event.key.keysym.scancode == SDL_SCANCODE_LEFT
				|| event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
					angle += step;
				else
					angle -= step;
				angle %= 360;
				freeImage(rotated);
				rotated = rotateImage(image, angle, 0);
				SDL_DestroyTexture(texture);
				SDL_FreeSurface(surface);
				surface = imageToSurface(rotated);
				texture = SDL_CreateTextureFromSurface(renderer, surface);
				if (texture == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
				draw(renderer, texture);
			}
			break;
		case SDL_KEYUP:
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = 0;
			break;
		}
	}
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	freeImage(rotated);
	return angle;
}

void draw(SDL_Renderer *renderer, SDL_Texture *texture) {
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

int rotateWithView(Image *image) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Window *window;
	window = SDL_CreateWindow("Rotate preview", 0, 0, 1, 1,
							  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Renderer *renderer =
		SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());

	SDL_SetWindowSize(window, 1000, 1000);
	// MAIN
	int theta = event_loop(renderer, image);
	// DESTRUCTION
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return theta;
}
