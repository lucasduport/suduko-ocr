#include "tools.h"
#include <stdio.h>

Image *newImage(st width, st height) {
	Image *image = (Image *)malloc(sizeof(Image));
	image->pixels = (uc *)malloc(width * height * sizeof(uc));
	image->width = width;
	image->height = height;
	return image;
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
