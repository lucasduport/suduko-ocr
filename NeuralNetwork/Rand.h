#ifndef RAND_H
#define RAND_H

#include <math.h>
#include <stdlib.h>

double r8_uniform_01(int *seed);
double r8_normal_01(int *seed);
double *r8vec_normal_ab_new(int n, double a, double b, int *seed);
double *r8vec_uniform_01_new(int n, int *seed);

#endif // RAND_H

