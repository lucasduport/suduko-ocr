#ifndef COST_H
#define COST_H

#define UNUSED(x) (void)(x)

#include <math.h>
#include <float.h>
#include "Tools.h"


ld CrossEntropy(ld *predicted, ld *expected, cui Size);
ld RMSE(ld *predicted, ld *expected, cui Size);
ld MAE(ld *predicted, ld *expected, cui Size);
ld MSE(ld *predicted, ld *expected, cui Size);

ld CrossEntropy_(cld predicted, cld expected);
ld RMSE_(cld predicted, cld expected);
ld MAE_(cld predicted, cld expected);
ld MSE_(cld predicted, cld expected);

ld none_(ld *arr, cui Size, cui ieme);
ld sigmoid_(ld *arr, cui Size, cui ieme);
ld softmax_(ld *arr, cui Size, cui ieme);
ld argmax_(ld *arr, cui Size, cui ieme);
ld step_(ld *arr, cui Size, cui ieme);
ld relu_(ld *arr, cui Size, cui ieme);
ld selu_(ld *arr, cui Size, cui ieme);
ld leakyrelu_(ld *arr, cui Size, cui ieme);

ld (*get_deriv(const char *name))(ld *arr, cui Size, cui ieme);
ld (*get_cost(const char *name))(ld *predicted, ld *expected,cui Size);
ld (*get_cost_deriv(const char *name))(ld predicted, ld expected);

const static struct {
  const char *name;
  ld (*func)(ld *arr, cui Size, cui ieme);
} deriv_map [] = {
  { "none", none_ },
  { "sigmoid", sigmoid_ },
  { "softmax", softmax_ },
  { "argmax", argmax_ },
  { "step", step_ },
  { "relu", relu_ },
  { "selu", selu_ },
  { "leakyrelu", leakyrelu_ }
};

const static struct {
  const char *name;
  ld (*func)(ld *predicted, ld *expected, cui Size);
} cost_map [] = {
  { "CrossEntropy", CrossEntropy },
  { "RMSE", RMSE },
  { "MAE", MAE },
  { "MSE", MSE }
};

const static struct {
  const char *name;
  ld (*func)(ld predicted, ld expected);
} cost_deriv_map [] = {
  { "CrossEntropy", CrossEntropy_ },
  { "RMSE", RMSE_ },
  { "MAE", MAE_ },
  { "MSE", MSE_ }
};

#endif // COST_H
