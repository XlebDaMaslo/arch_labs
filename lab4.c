#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

double
divided_diff(double* x, double* y, int n)
{
  if (n == 1)
  {
    return (y[1] - y[0]) / (x[1] - x[0]);
  }
  else
  {
    return (divided_diff(x + 1, y + 1, n - 1) - divided_diff(x, y, n - 1)) / (x[n] - x[0]);
  }
}

double
newton_interpolation(double* x, double* y, int n, double x_value)
{
  double result = y[0];
  double term = 1;
  for (int i = 1; i < n; i++)
  {
    term *= (x_value - x[i - 1]);
    result += divided_diff(x, y, i) * term;
  }
  return result;
}

int
main()
{
  FILE* file;

  file = fopen("in.txt", "r");
  if (file == NULL)
  {
    printf("Error opening the file.\n");
    return 1;
  }

  int n = 0;
  double x_val;
  while (fscanf(file, "%lf", &x_val) == 1)
  {
    n++;
  }

  fseek(file, 0, SEEK_SET);

  n = n / 2;

  double* x = (double*)malloc(n * sizeof(double));
  double* y = (double*)malloc(n * sizeof(double));

  for (int i = 0; i < n; i++)
  {
    fscanf(file, "%lf", &x[i]);
  }
  for (int i = 0; i < n; i++)
  {
    fscanf(file, "%lf", &y[i]);
  }

  fclose(file);

  for (int i = 0; i < n; i++)
  {
    printf("%.6lf ", x[i]);
  }
  printf("\n");
  for (int i = 0; i < n; i++)
  {
    printf("%.6lf ", y[i]);
  }
  printf("\n");

  double x_value = 1.0;

  double result = newton_interpolation(x, y, n, x_value);
  printf("f(%.1lf) = %.6lf\n", x_value, result);

  return 0;
}