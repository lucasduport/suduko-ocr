#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "Rand.h"

#define EPS 10E-10L

typedef unsigned int ui;
typedef const ui cui;
typedef long double ld;
typedef const ld cld;

inline ui max(ui a, ui b) {return a > b ? a : b;}
inline ui min(ui a, ui b) {return a > b ? b : a;}
void matr_display(ld *arr, cui s, cui cols);
ld *fvec_alloc(cui n, bool zInit);
char *cvec_alloc(cui n);
ld *fvec_rInit(cui n, cui conns, bool he_init);
inline ld absl(cld nb) {return (nb < 0) ? -nb : nb;}
void arr_shuffle(ld *arr[], ld *paired_arr[], cui Size);

#endif // TOOLS_H
