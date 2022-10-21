#include "Activations.h"

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include "Tools.h"

void relu(ld *input, ld *output, cui Size) {
	for(ld *o=output, *i=input; o<output+Size; o++, i++) *o = fmaxl(.0L, *i);
}

void leakyrelu(ld *input, ld *output, cui Size) {
	for (ui i=0; i<Size; i++) output[i] = fmaxl(.01 * input[i], input[i]);
}

void selu(ld *input, ld *output, cui Size) {
    cld alpha = 1.6732632423543772848170429916717;
    cld lambda = 1.0507009873554804934193349852946;
	for (ui i=0; i<Size; i++)
        output[i] = lambda * (input[i] > 0 ?
                              input[i] : alpha*expl(input[i])-alpha);
}

void none(ld *input, ld *output, cui Size) {
	for(ld *o=output, *i=input; o<output+Size; o++, i++) *o = *i;
}

void sigmoid(ld *input, ld *output, cui Size) {
	for(ld *o=output, *i=input; o<output+Size; o++, i++)
        *o = 1.0L/(1 + expl(-(*i)));
}

void softmax(ld *input, ld *output, cui Size) {
    ld Max = input[0];
    for(ld *i=input; i<input+Size; i++) Max = fmaxl((*i), Max);
    for(ld *i=input; i<input+Size; i++) *i -= Max;

	ld s = .0, expd[Size];
	for(ld *i=input, *e=expd; i<input+Size; i++, e++) {
        /*
        if (isnan(expl(input[i]))) {
            printf("\nnan in softmax caused by exp(%LF)\n", input[i]);
            exit(3);
        }
        */
		*e = expl(*i);
		s += *e;
	}

	for(ld *o=output, *e=expd; o<output+Size; o++, e++) {
        /*
        if (isnan(expd[i]/(s+EPS))) {
            puts("nan 2 in softmax");
            exit(4);
        }
        */
        *o = (*e)/(s+10E-8);
	}
}

void argmax(ld *input, ld *output, cui Size) {
	ui c = 0, p[Size];
	p[c] = 0;
	for (ui i=0; i<Size; i++) {
        if (input[i] > input[p[c]]) {
            c = 0;
            p[c] = i;
        }
        else if (absl(input[i] - input[p[c]]) < LDBL_EPSILON) {
            c++;
            p[c] = i;
        }
	}
	for (ui i=0; i<Size; i++) output[i] = .0L;
	for (ui i=0; i<c+1; i++) output[p[i]] = 1.0L;
}

void step(ld *input, ld *output, cui Size) {
	for (ui i=0; i<Size; i++) output[i] = (input[i] < .5L) ? .0L : 1.0L;
}

void (*get_activation(const char *name))(ld *input, ld *output, cui Size)
{
	for (ui i = 0; i < (sizeof(function_map) / sizeof(function_map[0])); i++)
		if (!strcmp(function_map[i].name, name)) return function_map[i].func;
	return NULL;
}


