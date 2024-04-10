//gcc lab6.c -o lab -lm
#include <stdio.h>
#include <math.h>

double f(double x) {
    return exp(-x * x);
}

double integrate_trapezoidal_N(double a, double b, int N) {
    double h = (b - a) / N;
    double sum = 0.5 * (f(a) + f(b));
    for (int i = 1; i < N; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    return h * sum;
}

double integrate_trapezoidal_eps(double a, double b, double epsilon) {
    int N = 1;
    double I1, I2;
    do {
        I1 = integrate_trapezoidal_N(a, b, N);
        N *= 2;
        I2 = integrate_trapezoidal_N(a, b, N);
    } while (fabs(I2 - I1) > epsilon);
    return I2;
}

int main() {
    double a, b, epsilon;
    int N;

    printf("Начало отрезка (a): ");
    scanf("%lf", &a);
    printf("Конец отрезка (b): ");
    scanf("%lf", &b);

    printf("Выберите метод:\n");
    printf("1. Задать количество разбиений (N)\n");
    printf("2. Задать погрешность (epsilon)\n");
    int choice;
    scanf("%d", &choice);

    double I;
    if (choice == 1) {
        printf("Введите количество разбиений (N): ");
        scanf("%d", &N);
        I = integrate_trapezoidal_N(a, b, N);
    }
    else if (choice == 2) {
        printf("Введите погрешность (epsilon): ");
        scanf("%lf", &epsilon);
        I = integrate_trapezoidal_eps(a, b, epsilon);
    }
    else {
        printf("Неверный выбор метода.\n");
        return 1;
    }

    printf("Численное значение интеграла: %lf\n", I);

    return 0;
}