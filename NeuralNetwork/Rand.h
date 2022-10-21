#ifndef RAND_H
#define RAND_H

#include <math.h>
#include <stdlib.h>

typedef unsigned int ui;
typedef long double ld;

ld r8_uniform_01(int *seed);
ld r8_normal_01(int *seed);
ld *r8vec_normal_ab_new(ui n, ld a, ld b, int *seed);
ld *r8vec_uniform_01_new(ui n, int *seed);

#endif // RAND_H

