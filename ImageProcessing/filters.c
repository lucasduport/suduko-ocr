#include "filters.h"
#include <err.h>
#include <math.h>

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
				if (j < 0) {
					j = -1;
					continue;
				}
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
				if (i < 0) {
					i = -1;
					continue;
				}
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
	int kernelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
	int kernelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
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

// better to use calibrate
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
