#include "tools.h"
#include <err.h>
#include <stdio.h>
#include <math.h>

float COS[360];
float SIN[360];

void initTrig() {
	static int trig_init = 0;
	if (trig_init) return;
	trig_init = 1;
	float c;
	for (int theta = 1; theta < 90; theta++) {
		c = cos(theta * PI / 180);
		COS[theta] = c;
		COS[360 - theta] = c;
		COS[180 + theta] = -c;
		COS[180 - theta] = -c;
		SIN[90 + theta] = c;
		SIN[90 - theta] = c;
		SIN[270 + theta] = -c;
		SIN[270 - theta] = -c;
	}
	SIN[180] = SIN[0] = COS[270] = COS[90] = 0;
	SIN[90] = COS[0] = 1;
	SIN[270] = COS[180] = -1;
}

uc *copyPixels(uc *pixels, st len) {
	uc *newPixels = malloc(sizeof(uc) * len);
	if (newPixels == NULL) errx(EXIT_FAILURE, "malloc failed");
	for (st i = 0; i < len; i++) newPixels[i] = pixels[i];
	return newPixels;
}

Image *newImage(st width, st height) {
	Image *image = (Image *)malloc(sizeof(Image));
	image->pixels = (uc *)malloc(width * height * sizeof(uc));
	image->width = width;
	image->height = height;
	return image;
}

Image *copyImage(Image *image) {
	Image *copy = malloc(sizeof(Image));
	if (copy == NULL) errx(EXIT_FAILURE, "malloc failed");
	copy->width = image->width;
	copy->height = image->height;
	copy->pixels = copyPixels(image->pixels, image->width * image->height);
	return copy;
}

void freeImage(Image *image) {
	free(image->pixels);
	free(image);
}

Point *newPoint(st x, st y) {
	Point *point = (Point *)malloc(sizeof(Point));
	point->x = x;
	point->y = y;
	return point;
}

Segment *newSegment(st x1, st y1, st x2, st y2, st theta, st r, st length) {
	Segment *segment = (Segment *)malloc(sizeof(Segment));
	segment->x1 = x1;
	segment->y1 = y1;
	segment->x2 = x2;
	segment->y2 = y2;
	segment->theta = theta;
	segment->r = r;
	segment->length = length;
	return segment;
}

void freeSegments(Segment **segments, int nb_segments) {
	for (int i = 0; i < nb_segments; i++) free(segments[i]);
}

Quadri *newQuadri(Point *p1, Point *p2, Point *p3, Point *p4) {
	Quadri *quadri = (Quadri *)malloc(sizeof(Quadri));
	quadri->p1 = p1;
	quadri->p2 = p2;
	quadri->p3 = p3;
	quadri->p4 = p4;
	return quadri;
}

void freeQuadri(Quadri *quadri) {
	free(quadri->p1);
	free(quadri->p2);
	free(quadri->p3);
	free(quadri->p4);
	free(quadri);
}

void printImage(Image *image) {
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	for (size_t y = 0; y < h; y++) {
		for (size_t x = 0; x < w; x++) printf("%02x ", pixels[y * w + x]);
		printf("\n");
	}
}
