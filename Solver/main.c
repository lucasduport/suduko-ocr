#include "solver.h"
#include "solver16.h"
#include "openFile.h"
#include "openFile16.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*examples of arrays

"image_01.jpeg": [5, 3, 0, 0, 7, 0, 0, 0, 0,
6, 0, 0, 1, 9, 5, 0, 0, 0,
0, 9, 8, 0, 0, 0, 0, 6, 0,
8, 0, 0, 0, 6, 0, 0, 0, 3,
4, 0, 0, 8, 0, 3, 0, 0, 1,
7, 0, 0, 0, 2, 0, 0, 0, 6,
0, 6, 0, 0, 0, 0, 2, 8, 0,
0, 0, 0, 4, 1, 9, 0, 0, 5,
0, 0, 0, 0, 8, 0, 0, 7, 9],
"image_02.jpeg": [0, 2, 0, 0, 0, 0, 6, 0, 9,
8, 5, 7, 0, 6, 4, 2, 0, 0,
0, 9, 0, 0, 0, 1, 0, 0, 0,
0, 1, 0, 6, 5, 0, 3, 0, 0,
0, 0, 8, 1, 0, 3, 5, 0, 0,
0, 0, 3, 0, 2, 9, 0, 8, 0,
0, 0, 0, 4, 0, 0, 0, 6, 0,
0, 0, 2, 8, 7, 0, 1, 3, 5,
1, 0, 6, 0, 0, 0, 0, 2, 0],
"image_03.jpeg": [0, 0, 0, 0, 0, 4, 5, 8, 0,
0, 0, 0, 7, 2, 1, 0, 0, 3,
4, 0, 3, 0, 0, 0, 0, 0, 0,
2, 1, 0, 0, 6, 7, 0, 0, 4,
0, 7, 0, 0, 0, 0, 2, 0, 0,
6, 3, 0, 0, 4, 9, 0, 0, 1,
3, 0, 6, 0, 0, 0, 0, 0, 0,
0, 0, 0, 1, 5, 8, 0, 0, 6,
0, 0, 0, 0, 0, 6, 9, 5, 0],
"image_04.jpeg": [7, 0, 8, 9, 0, 0, 0, 0, 2,
5, 1, 3, 0, 0, 2, 0, 0, 8,
0, 9, 2, 3, 1, 0, 0, 0, 7,
0, 5, 0, 0, 3, 0, 9, 0, 0,
1, 6, 0, 0, 2, 0, 0, 7, 5,
0, 0, 9, 0, 4, 0, 0, 6, 0,
9, 0, 0, 0, 8, 4, 2, 1, 0,
2, 0, 0, 6, 0, 0, 7, 4, 9,
4, 0, 0, 0, 0, 1, 5, 0, 3],
"image_05.jpeg": [0, 0, 0, 0, 0, 0, 0, 0, 0,
6, 0, 0, 0, 0, 0, 0, 0, 5,
9, 0, 5, 3, 0, 8, 4, 0, 6,
0, 1, 0, 0, 4, 0, 0, 7, 0,
0, 0, 0, 1, 0, 3, 0, 0, 0,
2, 0, 0, 0, 0, 0, 0, 0, 4,
4, 0, 0, 5, 2, 9, 0, 0, 1,
0, 0, 6, 0, 0, 0, 5, 0, 0,
0, 2, 0, 4, 0, 6, 0, 3, 0],
"image_06.jpeg": [0, 9, 6, 1, 0, 8, 5, 4, 0,
5, 0, 4, 0, 6, 2, 3, 8, 7,
2, 3, 0, 7, 4, 0, 9, 0, 1,
6, 4, 3, 0, 7, 9, 8, 1, 0,
0, 8, 0, 3, 0, 4, 6, 7, 9,
9, 0, 5, 8, 1, 0, 4, 2, 3,
0, 2, 9, 0, 8, 1, 0, 0, 6,
8, 0, 7, 5, 0, 3, 0, 9, 4,
4, 5, 0, 6, 9, 7, 2, 3, 0]
"image_01.jpeg": [5, 3, 0, 0, 7, 0, 0, 0, 0,
6, 0, 0, 1, 9, 5, 0, 0, 0,
0, 9, 8, 0, 0, 0, 0, 6, 0,
8, 0, 0, 0, 6, 0, 0, 0, 3,
4, 0, 0, 8, 0, 3, 0, 0, 1,
7, 0, 0, 0, 2, 0, 0, 0, 6,
0, 6, 0, 0, 0, 0, 2, 8, 0,
0, 0, 0, 4, 1, 9, 0, 0, 5,
0, 0, 0, 0, 8, 0, 0, 7, 9],
"image_02.jpeg": [0, 2, 0, 0, 0, 0, 6, 0, 9,
8, 5, 7, 0, 6, 4, 2, 0, 0,
0, 9, 0, 0, 0, 1, 0, 0, 0,
0, 1, 0, 6, 5, 0, 3, 0, 0,
0, 0, 8, 1, 0, 3, 5, 0, 0,
0, 0, 3, 0, 2, 9, 0, 8, 0,
0, 0, 0, 4, 0, 0, 0, 6, 0,
0, 0, 2, 8, 7, 0, 1, 3, 5,
1, 0, 6, 0, 0, 0, 0, 2, 0],
"image_03.jpeg": [0, 0, 0, 0, 0, 4, 5, 8, 0,
0, 0, 0, 7, 2, 1, 0, 0, 3,
4, 0, 3, 0, 0, 0, 0, 0, 0,
2, 1, 0, 0, 6, 7, 0, 0, 4,
0, 7, 0, 0, 0, 0, 2, 0, 0,
6, 3, 0, 0, 4, 9, 0, 0, 1,
3, 0, 6, 0, 0, 0, 0, 0, 0,
0, 0, 0, 1, 5, 8, 0, 0, 6,
0, 0, 0, 0, 0, 6, 9, 5, 0],
"image_04.jpeg": [7, 0, 8, 9, 0, 0, 0, 0, 2,
5, 1, 3, 0, 0, 2, 0, 0, 8,
0, 9, 2, 3, 1, 0, 0, 0, 7,
0, 5, 0, 0, 3, 0, 9, 0, 0,
1, 6, 0, 0, 2, 0, 0, 7, 5,
0, 0, 9, 0, 4, 0, 0, 6, 0,
9, 0, 0, 0, 8, 4, 2, 1, 0,
2, 0, 0, 6, 0, 0, 7, 4, 9,
4, 0, 0, 0, 0, 1, 5, 0, 3],
"image_05.jpeg": [0, 0, 0, 0, 0, 0, 0, 0, 0,
6, 0, 0, 0, 0, 0, 0, 0, 5,
9, 0, 5, 3, 0, 8, 4, 0, 6,
0, 1, 0, 0, 4, 0, 0, 7, 0,
0, 0, 0, 1, 0, 3, 0, 0, 0,
2, 0, 0, 0, 0, 0, 0, 0, 4,
4, 0, 0, 5, 2, 9, 0, 0, 1,
0, 0, 6, 0, 0, 0, 5, 0, 0,
0, 2, 0, 4, 0, 6, 0, 3, 0],
"image_06.jpeg": [0, 9, 6, 1, 0, 8, 5, 4, 0,
5, 0, 4, 0, 6, 2, 3, 8, 7,
2, 3, 0, 7, 4, 0, 9, 0, 1,
6, 4, 3, 0, 7, 9, 8, 1, 0,
0, 8, 0, 3, 0, 4, 6, 7, 9,
9, 0, 5, 8, 1, 0, 4, 2, 3,
0, 2, 9, 0, 8, 1, 0, 0, 6,
8, 0, 7, 5, 0, 3, 0, 9, 4,
4, 5, 0, 6, 9, 7, 2, 3, 0]
int array[9][9] =
	{
		{5,3,0,0,7,0,0,0,0},
		{6,0,0,1,9,5,0,0,0},
		{8,0,0,0,4,0,0,0,0},
		{0,0,0,0,8,0,0,0,0},
		{0,0,0,7,0,0,0,0,0},
		{0,0,0,0,2,6,0,0,9},
		{2,0,0,3,0,0,0,0,6},
		{0,0,0,2,0,0,9,0,0},
		{0,0,1,9,0,4,5,7,0}
	};

*/

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Missing argument\nPass a grid\n");
		return 0;
	}
	if (argc > 2) {
		printf("Too much argument\nOnly one grid is needed\n");
		return 0;
	}
	if (argc == 2) {
		const char *filename = argv[1];
		fileProcessing(filename);
	}
	//fileProcessing(argv[1]);
	return 0;
	/*int array2[9][9] =
	{
		{5,3,0,0,7,0,0,0,0},
		{6,0,0,1,9,5,0,0,0},
		{8,0,0,0,4,0,0,0,0},
		{0,0,0,0,8,0,0,0,0},
		{0,0,0,7,0,0,0,0,0},
		{0,0,0,0,2,6,0,0,9},
		{2,0,0,3,0,0,0,0,6},
		{0,0,0,2,0,0,9,0,0},
		{0,0,1,9,0,4,5,7,0}
	};*/
	/*printf("Grille entrée\n");
	for (int i=0; i<9; i++)
	{
		for (int j=0; j<9; j++)
		{
			printf("%d ",array[i][j]);
			if ((j+1)%3==0) printf(" ");
		}
		printf("\n");
		if ((i+1)%3==0) printf("\n");
	}*/
	/*int countMove = solver(array);
	//solver(array);
	printf("\n");
	printf("Grille résolue\n");
	if (countMove>0) printf("Resolu en %d coups\n\n",countMove);
	for (int i=0; i<9; i++)
	{
		for (int j=0; j<9; j++)
		{
			printf("%d ",array[i][j]);
			if ((j+1)%3==0) printf(" ");
		}
		printf("\n");
		if ((i+1)%3==0) printf("\n");
	}*/
}
