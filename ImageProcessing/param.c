#include "param.h"

static int nb_cells = 9;

void setNbCells(int _nb_cells)
{
	nb_cells = _nb_cells;
}

int getNbCells()
{
	return nb_cells;
}
