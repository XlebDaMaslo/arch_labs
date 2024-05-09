#include <stdio.h>
#include <stdlib.h>

#define DT 0.01 // Шаг по времени
#define MAX_DAYS 90 // Максимальное количество дней
#define N 2798170 // Население Новосибирской области

// Параметры модели SEIRD
#define ALPHA_E 0.999 
#define ALPHA_I 0.999 
#define K 0.042 
#define RHO 0.952 
#define BETA 0.999 
#define MU 0.0188 
#define CISOL 0 
#define E0 99 
#define R0 24

#define TAU 2
#define GAMMA 0

// Структура для хранения состояния модели
typedef struct {
    double S; // Восприимчивые
    double E; // Воздействованные
    double I; // Зараженные
    double R; // Выздоровевшие
    double D; // Умершие
} SEIRD;

// Функции для вычисления производных
double dSdt(double S, double E, double I, double R, double t);
double dEdt(double S, double E, double I, double t);
double dIdt(double E, double I, double t);
double dRdt(double E, double I, double R, double t);
double dDdt(double I, double t);

int main() {
    SEIRD state = {N - E0 - R0, E0, 0, R0, 0}; // Начальное состояние модели SEIRD
    double t = 0.0; // Начальное время
    double fk; // Количество выявленных инфицированных за сутки

    // Открываем файл для записи результатов
    FILE *fp = fopen("results.txt", "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }
    fprintf(fp, "Время\t\tS\t\t\tE\t\tI\t\tR\t\tD\n");
    while (t <= MAX_DAYS) { // Цикл моделирования до максимального количества дней
        fk = K * state.E;
        fprintf(fp, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, state.S, state.E, state.I, state.R, state.D, fk);

        double dS = dSdt(state.S, state.E, state.I, state.R, t);
        double dE = dEdt(state.S, state.E, state.I, t);
        double dI = dIdt(state.E, state.I, t);
        double dR = dRdt(state.E, state.I, state.R, t);
        double dD = dDdt(state.I, t);

        state.S += dS * DT; // Обновление числа восприимчивых
        state.E += dE * DT; // Обновление числа воздействованных
        state.I += dI * DT; // Обновление числа зараженных
        state.R += dR * DT; // Обновление числа выздоровевших
        state.D += dD * DT; // Обновление числа умерших

        t += DT; // Увеличение времени на шаг DT
    }
    fprintf(fp, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, state.S, state.E, state.I, state.R, state.D, fk);
    fclose(fp);
    
    // Создание отдельных файлов для графиков каждой переменной
    char* variables[] = {"S", "E", "I", "R", "D", "fk"};
    for (int i = 0; i < 6; i++) {
        // Создание файла скрипта для графика
        char plotFileName[20];
        sprintf(plotFileName, "plot_%s.gnu", variables[i]);
        FILE *plotFile = fopen(plotFileName, "w");
        if (plotFile == NULL) {
            printf("Error opening plot script file for %s.\n", variables[i]);
            return 1;
        }
        fprintf(plotFile, "set terminal png\n"); // Установка формата вывода графика в PNG
        fprintf(plotFile, "set output 'plot_%s.png'\n", variables[i]); // Установка имени выходного файла
        fprintf(plotFile, "set title 'SEIRD Model - %s'\n", variables[i]); // Установка заголовка графика
        fprintf(plotFile, "set xlabel 'Time'\n"); // Установка подписи оси X
        fprintf(plotFile, "set ylabel '%s'\n", variables[i]); // Установка подписи оси Y
        fprintf(plotFile, "plot 'results.txt' using 1:%d with lines title '%s'\n", i + 2, variables[i]); // Построение графика
        fclose(plotFile); // Закрытие файла скрипта

        // Запуск gnuplot с файлом скрипта
        char command[50];
        sprintf(command, "gnuplot %s", plotFileName);
        system(command);
    }
    return 0;
}

// Функции для вычисления изменения каждой переменной
double a(double t) {
    return 2.5; // Индекс самоизоляции, основанный на данных Яндекса
}

double c(double t) {
    return 1 + CISOL * (1 - (2 / 5) * a(t));
}

double dSdt(double S, double E, double I, double R, double t) {
    return (-c(t - TAU) * ((ALPHA_I * S * I)/N + (ALPHA_E * S * E)/N) + GAMMA * R);
}

double dEdt(double S, double E, double I, double t) {
    return c(t - TAU) * ((ALPHA_I * S * I)/N + (ALPHA_E * S * E)/N) - (K + RHO) * E;
}

double dIdt(double E, double I, double t) {
    return K * E - BETA * I - MU * I;
}

double dRdt(double E, double I, double R, double t) {
    return BETA * I + RHO * E - GAMMA * R;
}

double dDdt(double I, double t) {
    return MU * I;
}