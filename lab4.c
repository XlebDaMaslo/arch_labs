#include <stdio.h>
#include <stdlib.h>

#define MAX 100

double x[MAX], y[MAX][MAX];

void newton_interpolation(int n) {
    for (int i = 1; i < n; i++) {
        for (int j = n-1; j >= i; j--) {
            y[j][i] = (y[j][i-1] - y[j-1][i-1]) / (x[j] - x[j-i]);
        }
    }
}

double newton_polynomial(double val, int n) {
    double sum = y[n-1][n-1];
    double prod = 1;

    for (int i = n-2; i >= 0; i--) {
        prod *= (val - x[i]);
        sum += (prod * y[i][i]);
    }

    return sum;
}

int main() {
    FILE *file = fopen("in.txt", "r");
    if (file == NULL) {
        printf("Не удалось открыть файл.\n");
        return 1;
    }

    int n = 0;
    while (fscanf(file, "%lf %lf", &x[n], &y[n][0]) != EOF) {
        n++;
    }

    fclose(file);

    newton_interpolation(n);

    double val = 1.0;
    printf("Значение полинома N(1) = %.6lf\n", newton_polynomial(val, n));

    return 0;
}
