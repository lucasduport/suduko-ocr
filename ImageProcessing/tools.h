#pragma once

#include <stdlib.h>

#define FILENAME "Images/image_06.jpeg"
#define PI 3.141592654

typedef unsigned char uc;
typedef size_t st;

typedef struct
{
    uc *pixels;
    st width;
    st height;
} Image;

typedef struct
{
    st x, y;
} Point;

typedef struct
{
    st x1, y1, x2, y2, theta, r, length;
} Segment;

typedef struct
{
    Point *p1, *p2, *p3, *p4;
} Quadri;

Image *newImage(st width, st height);
void freeImage(Image *image);
Point *newPoint(st x, st y);
Segment *newSegment(st x1, st y1, st x2, st y2, st theta, st r, st length);
void freeSegments(Segment **segments, int nb_segments);
Quadri *newQuadri(Point *p1, Point *p2, Point *p3, Point *p4);
void freeQuadri(Quadri *quadri);
void printImage(Image *image);
