#include "Rand.h"

double r8_uniform_01(int *seed)
{
      const int i4_huge = 2147483647;
      int k;
      double r;
      k = *seed/127773;
      *seed = 16807 * (*seed - k * 127773) - k * 2836;
      if (*seed < 0) *seed += i4_huge;
      r = (double) (*seed) * 4.656612875E-10;
      return r;
}

double r8_normal_01(int *seed)
{
      double r1, r2, x;
      const double r8_pi = 3.141592653589793;
      r1 = r8_uniform_01(seed);
      r2 = r8_uniform_01(seed);
      x = sqrt(-2.0 * log(r1)) * cos(2.0 * r8_pi * r2);
      return x;
}

double *r8vec_normal_ab_new(int n, double b, double c, int *seed)
{
	printf("b = %f, c = %f\n", b, c);
  int i, m, x_hi, x_lo;
  double *r, *x;
  const double r8_pi = 3.141592653589793;

  x = (double*) malloc(n * sizeof(double));

  x_lo = 1;
  x_hi = n;

  if (x_hi - x_lo + 1 == 1) {
    r = r8vec_uniform_01_new(2, seed);
    x[x_hi-1] = sqrt(-2.0 * log(r[0])) * cos(2.0 * r8_pi * r[1]);
    free (r);
  }
  else if((x_hi - x_lo + 1) % 2 == 0) {
    //Even Number needed
    m = (x_hi - x_lo + 1)/2;
    r = r8vec_uniform_01_new(2*m, seed);
    for (i = 0; i <= 2*m-2; i = i + 2) {
      x[x_lo+i] = sqrt(-2.0 * log(r[i])) * sin(2.0 * r8_pi * r[i+1]);
      x[x_lo+i-1] = sqrt(-2.0 * log(r[i])) * cos(2.0 * r8_pi * r[i+1]);
      //printf("s1 = %f; s2 = %f\n", x[x_lo+i-1], x[x_lo+i]);
	printf("r[%d] = %f\n", x_lo+i-1, r[x_lo+i-1]);
	printf("r[%d] = %f\n", x_lo+i, r[x_lo+i]);
    }
    free (r);
  }
  else {
    //Odd Number needed
    x_hi = x_hi - 1;
    m = (x_hi - x_lo + 1)/2 + 1;
    r = r8vec_uniform_01_new(2*m, seed);
    for (i = 0; i <= 2*m-4; i = i + 2) {
      x[x_lo+i-1] = sqrt (-2.0 * log (r[i])) * cos(2.0 * r8_pi * r[i+1]);
      x[x_lo+i] = sqrt(-2.0 * log(r[i])) * sin(2.0 * r8_pi * r[i+1]);
    }

    i = 2*m - 2;
    x[x_lo+i-1] = sqrt(-2.0 * log(r[i])) * cos(2.0 * r8_pi * r[i+1]);
    free (r);
  }

  for (i = 0; i < n; i++) {
	printf("%d : before = %f, ", i, x[i]);
	x[i] = b + c * x[i];
	printf("after = %f\n", x[i]);
  }
  return x;
}

double *r8vec_uniform_01_new (int n, int *seed)
{
      int i, k;
      const int i4_huge = 2147483647;
      double *r;
      r = (double*) malloc(n*sizeof(double));

      for (i=0; i < n; i++) {
        k = *seed/127773;
        *seed = 16807 * (*seed - k * 127773 ) - k * 2836;
        if (*seed < 0) *seed = *seed + i4_huge;
        r[i] = (double) (*seed) * 4.656612875E-10;
      }
      return r;
}
