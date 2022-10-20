#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#include <math.h>
#include <float.h>
#include "Tools.h"

void sigmoid(ld *input, ld *output, cui Size);
void softmax(ld *input, ld *output, cui Size);
void argmax(ld *input, ld *output, cui Size);
void step(ld *input, ld *output, cui Size);
void relu(ld *input, ld *output, cui Size);
void selu(ld *input, ld *output, cui Size);
void leakyrelu(ld *input, ld *output, cui Size);
void none(ld *input, ld *output, cui Size);

void (*get_activation(const char *name))(ld *input, ld *output, cui Size);

const static struct {
  const char *name;
  void (*func)(ld *input, ld *output, cui Size);
} function_map [] = {
  { "none", none },
  { "sigmoid", sigmoid },
  { "softmax", softmax },
  { "argmax", argmax },
  { "step", step },
  { "relu", relu },
  { "selu", selu },
  { "leakyrelu", leakyrelu }
};

#endif // ACTIVATIONS_H
