#include "transformImage.h"

uc lerp(uc *pxls, float x, float y, st w) {
	// x1 <= x <= x2
	// y1 <= y <= y2
	st x1 = x;
	st x2 = x1 + 1;
	st y1 = y;
	st y2 = y1 + 1;
	float w_x1 = x2 - x;
	float w_x2 = x - x1;
	float w_y1 = y2 - y;
	float w_y2 = y - y1;
	float val = 0;
	val += pxls[y1 * w + x1] * w_x1 * w_y1;
	val += pxls[y1 * w + x2] * w_x2 * w_y1;
	val += pxls[y2 * w + x1] * w_x1 * w_y2;
	val += pxls[y2 * w + x2] * w_x2 * w_y2;
	return (uc)(val + 0.5);
}

void resizeImage(Image *image, st new_w, st new_h) {
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	uc *new_pixels = malloc(sizeof(uc) * new_w * new_h);
	float ratio_w = (float)w / new_w;
	float ratio_h = (float)h / new_h;
	float x, y;
	for (st new_y = 0; new_y < new_h; new_y++) {
		y = new_y * ratio_h;
		for (st new_x = 0; new_x < new_w; new_x++) {
			x = new_x * ratio_w;
			if (x < 0 || x + 1 >= w || y < 0 || y + 1 >= h) {
				new_pixels[new_y * new_w + new_x] = 0;
				continue;
			}
			new_pixels[new_y * new_w + new_x] = lerp(pixels, x, y, w);
		}
	}
	free(pixels);
	image->pixels = new_pixels;
	image->width = new_w;
	image->height = new_h;
}

void autoResize(Image *image, st max_w, st max_h) {
	if (image->width <= max_w && image->height <= max_h) return;
	// if ratio_w > ratio_h, then we resize by width
	float ratio_w = (float)image->width / max_w;
	float ratio_h = (float)image->height / max_h;
	if (ratio_w > ratio_h) resizeImage(image, max_w, image->height / ratio_w);
	else resizeImage(image, image->width / ratio_h, max_h);
}

Image *extractGrid(Image *image, Quad *quad, st new_w, st new_h) {
	// p1 : ul, p2 : ur,
	// p3 : dl, p4 : dr
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	Image *new_image = newImage(new_w, new_h);
	uc *new_pixels = new_image->pixels;
	float input[3] = {0, 0, 1};
	float mat[3][3];
	getTransformMatrix(quad, new_w, new_h, mat);
	float res[3];
	float x, y;
	for (st new_y = 0; new_y < new_h; new_y++) {
		input[1] = new_y;
		for (st new_x = 0; new_x < new_w; new_x++) {
			input[0] = new_x;
			matMul33_31(mat, input, res);
			x = res[0] / res[2];
			y = res[1] / res[2];
			if (x < 0 || x + 1 >= w || y < 0 || y + 1 >= h) {
				new_pixels[new_y * new_w + new_x] = 0;
				continue;
			}
			new_pixels[new_y * new_w + new_x] = lerp(pixels, x, y, w);
		}
	}
	return new_image;
}

Image *rotateImage(Image *image, int angle, uc background_color) {
	uc *pixels = image->pixels;
	int w = image->width, h = image->height;
	// all corners ((0, 0), (w, 0), (0, h), (w, h)) must be in the new image
	float _cos = COS[angle];
	float _sin = SIN[angle];
	int new_w = fabs(w * _cos) + fabs(h * _sin);
	int new_h = fabs(w * _sin) + fabs(h * _cos);
	Image *new_image = newImage(new_w, new_h);
	uc *new_pixels = new_image->pixels;
	float new_x0, new_y0;
	for (int new_y = 0; new_y < new_h; new_y++) {
		new_y0 = new_y - new_h / 2.;
		for (int new_x = 0; new_x < new_w; new_x++) {
			new_x0 = new_x - new_w / 2.;
			float x = (new_x0) * _cos - (new_y0) * _sin + w / 2;
			float y = (new_x0) * _sin + (new_y0) * _cos + h / 2;
			if (x < 0 || x + 1 >= w || y < 0 || y + 1 >= h) {
				new_pixels[new_y * new_w + new_x] = background_color;
				continue;
			}
			new_pixels[new_y * new_w + new_x] = lerp(pixels, x, y, w);
		}
	}
	return new_image;
}

Quad *rotateQuad(Quad *quad, int theta, Image *image, Image *rotated) {
	int w = image->width, h = image->height;
	int new_w = rotated->width, new_h = rotated->height;
	float _cos = COS[theta], _sin = SIN[theta];
	float new_x, new_y, new_x0, new_y0;
	Point *ps[4] = {quad->p1, quad->p2, quad->p3, quad->p4};
	Point *n_ps[4];
	float x, y;
	for (st i = 0; i < 4; i++) {
		new_x = ps[i]->x, new_y = ps[i]->y;
		new_x0 = new_x - new_w / 2., new_y0 = new_y - new_h / 2.;
		x = (new_x0) * _cos - (new_y0) * _sin + w / 2;
		y = (new_x0) * _sin + (new_y0) * _cos + h / 2;
		n_ps[i] = newPoint(x + 0.5, y + 0.5);
	}
	return newQuad(n_ps[0], n_ps[1], n_ps[2], n_ps[3]);
}

// it integrate the number in the image with the origin : origin.
// only if the pixel of number is less than 10
// use placeDigit() instead (adapted from this one)
void integrateNumber(Image *image, Image *number, Point *origin) {
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	uc *integrated_pixels = number->pixels;
	st integrated_w = number->width, integrated_h = number->height;
	for (st y = 0; y < integrated_h; y++) {
		for (st x = 0; x < integrated_w; x++) {
			st new_x = x + origin->x;
			st new_y = y + origin->y;
			if (new_x < 0 || new_x >= w || new_y < 0 || new_y >= h
				|| integrated_pixels[y * integrated_w + x] > 20)
				continue;
			pixels[new_y * w + new_x] = integrated_pixels[y * integrated_w + x];
		}
	}
}
