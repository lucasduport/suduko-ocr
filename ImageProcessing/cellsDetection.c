#include "cellsDetection.h"
#include "image.h"
#include "tools.h"

int *buildHistoH(Image *image)
{
    st width = image->width;
    st height = image->height;
	uc *channel = image->channels[0];
	int *histo_h = (int *)malloc(sizeof(int)*height);
	if (histo_h == NULL)
		errx(EXIT_FAILURE, "malloc failed");

	for (st y = 0; y < width; y++)
	{
		int sum = 0;
		for (st x = 0; x < height; x++)
		    sum += channel[y * width + x];
		histo_h[y] = sum;
	}

	return histo_h;

}

int *buildHistoV(Image *image)
{
    st width = image->width;
	st height = image->height;
	uc *channel = image->channels[0];
	int *histo_v = (int *)malloc(width * sizeof(int));
	if (histo_v == NULL)
		errx(EXIT_FAILURE, "malloc failed");

	for (st x = 0; x < height; x++)
	{
		int sum = 0;
		for (st y = 0; y < width; y++)
		    sum += channels[y * width + x];
		histo_v[x] = sum;
	}

	return histo_v;
}

void detectNbLines(int *histo, st len, st nb_lines, int **coords, int **values)
{
    *values = (int *)malloc(sizeof(int) * (nb_lines - 1));
	if (*values == NULL)
		errx(EXIT_FAILURE, "malloc failed");
	float cell_size = (float)len / nb_lines;
	int range = cell_size / 4;
	for (int i = 1; i < nb_lines; i++)
	{
		int max = 0;
		int x0 = i * cell_size;
		int x_max = x0;
		for (int x = x0 - range; x < x0 + range; x++)
		{
		    if (histo[x] > max)
			{
				max = histo[x];
				x_max = x;
			}
		}
		*values[i] = max;
		*coords[i] = x_max;
	}
}

int getGridDimension(
