#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

double
divided_diff (double *x, double *y, int n)
{
  if (n == 1)
    {
      return (y[1] - y[0]) / (x[1] - x[0]);
    }
  else
    {
      return (divided_diff (x + 1, y + 1, n - 1) - divided_diff (x, y, n - 1))
             / (x[n] - x[0]);
    }
}

double
newton_interpolation (double *x, double *y, int n, double x_value)
{
  double result = y[0];
  double term = 1;
  for (int i = 1; i < n; i++)
    {
      term *= (x_value - x[i - 1]);
      result += divided_diff (x, y, i) * term;
    }
  return result;
}

int
main ()
{
    FILE *file;
    double x[5], y[5];
    int n = 5;

    file = fopen("in.txt", "r");
    if (file == NULL) {
        printf("Ошибка при открытии файла.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%lf", &x[i]);
    }
    for (int i = 0; i < n; i++) {
        fscanf(file, "%lf", &y[i]);
    }

    fclose(file);

    for (int i = 0; i < n; i++) {
        printf("%.2lf ", x[i]);
    }
    printf("\n");

  //   int n = 3;

  //   double x[] = { 1, 2, 3 };
  //   double y[] = { 1, 4, 9 };

  double x_value = 1.0;

  double result = newton_interpolation (x, y, n, x_value);
  printf ("f(%.1lf) = %.6lf\n", x_value, result);

  return 0;
}