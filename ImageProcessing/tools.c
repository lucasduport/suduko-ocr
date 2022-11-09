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

Point *newPoint(st x, st y) {
	Point *point = (Point *)malloc(sizeof(Point));
	if (point == NULL) errx(EXIT_FAILURE, "malloc failed");
	point->x = x;
	point->y = y;
	return point;
}

Segment *newSegment(st x1, st y1, st x2, st y2, st theta, st r, st length) {
	Segment *segment = (Segment *)malloc(sizeof(Segment));
	if (segment == NULL) errx(EXIT_FAILURE, "malloc failed");
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
	if (quadri == NULL) errx(EXIT_FAILURE, "malloc failed");
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
