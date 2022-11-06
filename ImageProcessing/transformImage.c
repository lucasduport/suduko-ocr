#include "transformImage.h"
#include "matrices.h"
#include "tools.h"
#include <math.h>
#include <stdio.h>

void invertImage(Image *image) {
	uc *pixels = image->pixels;
	st len = image->width * image->height;
	for (size_t i = 0; i < len; i++) pixels[i] = 255 - pixels[i];
}

void thresholdCells(Image *image) {
	int grid_size = image->width;
	int cell_size = grid_size / 9;
	uc *pixels = image->pixels;
	uc *new_pixels = copyPixels(pixels, grid_size * grid_size);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			int x1 = i * cell_size - 10;
			int x2 = (i + 1) * cell_size + 10;
			int y1 = j * cell_size - 10;
			int y2 = (j + 1) * cell_size + 10;
			if (x1 < 0) x1 = 0;
			if (x2 > grid_size) x2 = grid_size;
			if (y1 < 0) y1 = 0;
			if (y2 > grid_size) y2 = grid_size;
			int min = 255;
			int max = 0;
			for (int x = x1; x < x2; x++) {
				for (int y = y1; y < y2; y++) {
					int value = pixels[y * grid_size + x];
					if (value < min) min = value;
					if (value > max) max = value;
				}
			}
			int threshold = min * 0.7 + max * 0.3;
			for (int x = i * cell_size; x < (i + 1) * cell_size; x++) {
				for (int y = j * cell_size; y < (j + 1) * cell_size; y++) {
					int value = pixels[y * grid_size + x];
					if (value < threshold) new_pixels[y * grid_size + x] = 255;
					else new_pixels[y * grid_size + x] = 0;
				}
			}
		}
	}
	free(pixels);
	image->pixels = new_pixels;
}

void gaussianBlur(Image *image) {
	int kernel[5][5] = {{2, 4, 5, 4, 2},
						{4, 9, 12, 9, 4},
						{5, 12, 15, 12, 5},
						{4, 9, 12, 9, 4},
						{2, 4, 5, 4, 2}};
	uc *pixels = image->pixels;
	int w = image->width;
	int h = image->height;
	uc *newPixels = malloc(sizeof(uc) * w * h);
	if (newPixels == NULL) errx(EXIT_FAILURE, "malloc failed");
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int sum = 0;
			int weight = 0;
			for (int i = -2; i <= 2; i++) {
				for (int j = -2; j <= 2; j++) {
					int x2 = x + j;
					int y2 = y + i;
					if (x2 < 0 || x2 >= w || y2 < 0 || y2 >= h) continue;
					sum += pixels[y2 * w + x2] * kernel[i + 2][j + 2];
					weight += kernel[i + 2][j + 2];
				}
			}
			newPixels[y * w + x] = sum / weight;
		}
	}
	free(pixels);
	image->pixels = newPixels;
}

void calibrateImage(Image *image, int radius) {
	int w = image->width, h = image->height;
	uc *pixels = image->pixels;
	uc *copy_pixels = copyPixels(pixels, w * h);
	uc maxs[w];
	uc mins[w];
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			uc max = 0;
			uc min = 255;
			for (int j = y - radius; j <= y + radius; j++) {
				if (j < 0) {j = -1; continue;}
				if (j >= h) break;
				uc pixel = copy_pixels[j * w + x];
				if (pixel > max) max = pixel;
				if (pixel < min) min = pixel;
			}
			maxs[x] = max;
			mins[x] = min;
		}
		for (int x = 0; x < w; x++) {
			uc min = 255;
			uc max = 0;
			for (int i = x - radius; i <= x + radius; i++) {
				if (i < 0) {i = -1; continue;}
				if (i >= w) break;
				uc pixel = maxs[i];
				if (pixel > max) max = pixel;
				pixel = mins[i];
				if (pixel < min) min = pixel;
			}
			uc pixel = copy_pixels[y * w + x];
			if (min == max) {
				pixels[y * w + x] = 255;
			} else {
				pixels[y * w + x] = (pixel - min) * 255 / (max - min);
			}
		}
	}
	free(copy_pixels);
}

void sobelFilter(Image *image) {
	int kernelX[3][3] = {{-1, 0, 1},
						 {-2, 0, 2},
						 {-1, 0, 1}};
	int kernelY[3][3] = {{-1, -2, -1},
						 {0, 0, 0},
						 {1, 2, 1}};
	uc *pixels = image->pixels;
	int w = image->width;
	int h = image->height;
	int *gradients = malloc(sizeof(int) * w * h);
	if (gradients == NULL) errx(EXIT_FAILURE, "malloc failed");
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int sumX = 0;
			int sumY = 0;
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					int x2 = x + j;
					int y2 = y + i;
					if (x2 < 0 || x2 >= w || y2 < 0 || y2 >= h) continue;
					sumX += pixels[y2 * w + x2] * kernelX[i + 1][j + 1];
					sumY += pixels[y2 * w + x2] * kernelY[i + 1][j + 1];
				}
			}
			gradients[y * w + x] = sqrt(sumX * sumX + sumY * sumY);
		}
	}
	int gradientMax = 0;
	for (int i = 0; i < w * h; i++) {
		if (gradients[i] > gradientMax) gradientMax = gradients[i];
	}
	for (int i = 0; i < w * h; i++) {
		pixels[i] = gradients[i] * 255 / gradientMax;
	}
	free(gradients);
}

void saturateImage(Image *image) {
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	st histo[256] = {0};

	for (st y = 0; y < h; y++)
		for (st x = 0; x < w; x++) histo[pixels[y * w + x]]++;

	st median = 0;
	for (st i = 255; i >= 0; i--) {
		median += histo[i];
		if (median > w * h * 0.875) {
			median = i;
			break;
		}
	}
	if (median == 255) median = 254;
	if (median == 0) median = 1;
	for (st y = 0; y < h; y++)
		for (st x = 0; x < w; x++)
			pixels[y * w + x] = pixels[y * w + x] >= median ? 255 : 0;
	return;
}

void resizeImage(Image *image, st new_w, st new_h) {
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	uc *new_pixels = malloc(sizeof(uc) * new_w * new_h);
	float ratio_w = (float)w / new_w;
	float ratio_h = (float)h / new_h;
	for (st new_y = 0; new_y < new_h; new_y++) {
		float y = new_y * ratio_h;
		st upper_y = y;
		st lower_y = upper_y + 1;
		for (st new_x = 0; new_x < new_w; new_x++) {
			float x = new_x * ratio_w;
			st left_x = x;
			st right_x = left_x + 1;
			if (left_x < 0 || right_x >= w || upper_y < 0 || lower_y >= h) {
				new_pixels[new_y * new_w + new_x] = 0;
				continue;
			}
			float weight_left = 1 - (x - left_x);
			float weight_right = 1 - weight_left;
			float weight_top = 1 - (y - upper_y);
			float weight_bottom = 1 - weight_top;
			float value = 0;
			value += pixels[upper_y * w + left_x] * weight_left * weight_top;
			value += pixels[upper_y * w + right_x] * weight_right * weight_top;
			value += pixels[lower_y * w + left_x] * weight_left * weight_bottom;
			value +=
				pixels[lower_y * w + right_x] * weight_right * weight_bottom;
			new_pixels[new_y * new_w + new_x] = (uc)(value + 0.5);
		}
	}
	free(pixels);
	image->pixels = new_pixels;
	image->width = new_w;
	image->height = new_h;
}

void autoResize(Image *image, st maw_w, st max_h) {
	if (image->width <= maw_w && image->height <= max_h)
		return;
	// if ratio_w > ratio_h, then we resize by width
	float ratio_w = (float)image->width / maw_w;
	float ratio_h = (float)image->height / max_h;
	if (ratio_w > ratio_h)
		resizeImage(image, maw_w, image->height / ratio_w);
	else
		resizeImage(image, image->width / ratio_h, max_h);
}

Image *extractGrid(Image *image, Quadri *quadri, st new_w, st new_h) {
	// p1 : ul, p2 : ur,
	// p3 : dl, p4 : dr
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	Image *new_image = newImage(new_w, new_h);
	uc *new_pixels = new_image->pixels;
	float mat33[3][3];
	getTransformMatrix(quadri, new_w, new_h, mat33);
	float mat31[3];
	mat31[2] = 1;
	float res[3];
	for (st new_y = 0; new_y < new_h; new_y++) {
		for (st new_x = 0; new_x < new_w; new_x++) {
			mat31[0] = new_x;
			mat31[1] = new_y;
			// mat31[2] = 1;
			matMul33_31(mat33, mat31, res);
			float x = res[0] / res[2];
			float y = res[1] / res[2];
			if (x < 0 || x >= w - 1 || y < 0 || y >= h - 1) {
				new_pixels[new_y * new_w + new_x] = 0;
				continue;
			}
			st left_x = x;
			st right_x = left_x + 1;
			st upper_y = y;
			st lower_y = upper_y + 1;
			float weight_left = 1 - (x - left_x);
			float weight_right = 1 - weight_left;
			float weight_top = 1 - (y - upper_y);
			float weight_bottom = 1 - weight_top;
			float value = 0;
			value += pixels[upper_y * w + left_x] * weight_left * weight_top;
			value += pixels[upper_y * w + right_x] * weight_right * weight_top;
			value += pixels[lower_y * w + left_x] * weight_left * weight_bottom;
			value +=
				pixels[lower_y * w + right_x] * weight_right * weight_bottom;
			new_pixels[new_y * new_w + new_x] = (uc)(value + 0.5);
		}
	}
	return new_image;
}

Image *rotateImage(Image *image, int angleD, uc background_color) {
	double angle = angleD * PI / 180;
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	// all corners ((0, 0), (w, 0), (0, h), (w, h)) must be in the new image
	float _cos = cos(angle);
	float _sin = sin(angle);
	st new_w = fabs(w * cos(angle)) + fabs(h * sin(angle));
	st new_h = fabs(w * sin(angle)) + fabs(h * cos(angle));
	Image *new_image = newImage(new_w, new_h);
	uc *new_pixels = new_image->pixels;
	for (st new_y = 0; new_y < new_h; new_y++) {
		for (st new_x = 0; new_x < new_w; new_x++) {
			float x = ((float)new_x - new_w / 2) * _cos
					  - ((float)new_y - new_h / 2) * _sin + w / 2;
			float y = ((float)new_x - new_w / 2) * _sin
					  + ((float)new_y - new_h / 2) * _cos + h / 2;
			if (x < 0 || x >= w - 1 || y < 0 || y >= h - 1) {
				new_pixels[new_y * new_w + new_x] = background_color;
				continue;
			}
			st upper_y = (st)y;
			st lower_y = upper_y + 1;
			st left_x = (st)x;
			st right_x = left_x + 1;
			float weight_left = 1 - (x - left_x);
			float weight_right = 1 - weight_left;
			float weight_top = 1 - (y - upper_y);
			float weight_bottom = 1 - weight_top;
			float value = 0;
			value += pixels[upper_y * w + left_x] * weight_left * weight_top;
			value += pixels[upper_y * w + right_x] * weight_right * weight_top;
			value += pixels[lower_y * w + left_x] * weight_left * weight_bottom;
			value +=
				pixels[lower_y * w + right_x] * weight_right * weight_bottom;
			new_pixels[new_y * new_w + new_x] = (uc)(value + 0.5);
		}
	}
	return new_image;
}

// it integrate the number in the image with the origin : origin.
// only if the pixel of number is less than 10
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

/*
void resize(Image *image, st new_w, st new_h) {
	Image *new_image = resizeImage(image, new_w, new_h);
	free(image->pixels);
	image->pixels = new_image->pixels;
	image->width = new_image->width;
	image->height = new_image->height;
	free(new_image);
}
*/
