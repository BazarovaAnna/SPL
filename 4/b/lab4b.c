#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "lib_ll.h"
#include "lib_ho.h"


/**
 * Функция для вывода list'а через пробелы
 */
void fun_print_with_space(int x) {
    printf("%d ", x);
}

/**
 * Функция для вывода list'а через переносы строки
 */
void fun_print_with_n(int x){
    printf("%d\n", x);
}

/**
 * функция возводит в квадрат
 */
int fun_square(int x){
    return x*x;
}

/**
 * Функция возводит в куб
 */
int fun_cube(int x){
    return x*x*x;
}

/**
 * Функция возввращает сумму элементов
 */
int fun_sum(int acc, int x){
    return acc + x;
}

/**
 * Функция для поиска максимального значения в list
 */
int fun_max(int acc, int x){
    return acc > x ? acc : x;
}

/**
 * Функция для поиска минимального в list
 */
int fun_min(int acc, int x){
    return acc < x ? acc : x;
}

/**
 * Функция возвращает модуль числа
 */
int fun_abs(int x){
    return abs(x);
}

/**
 * Функция для степент 2
 */
int fun_pow_two(int x){
    return x*2;
}

int main() {
    int e;  
    struct list* list = NULL;
    printf("Введите - 1 чтобы считать из файла\n"
           "        - 2 чтобы считать с консоли\n");
    int k;
    scanf("%d", &k);
    switch (k){
        case 1: {
            load(&list, "input.txt");
            printf("Считали список из файла 'input.txt'.\n");
            break;
        }
        case 2: {
            printf("Введите список:\n");
            while (1){
                int s = scanf("%d", &e);
                if (s == EOF){
                    break;
                }
                if (s != 1){
                    printf("%c - не является числом. Введите, пожалуйста число.\n", getchar());
                } else {
                    printf("Элемент '%d' добавлен в список\n", e);
                    list_add_back(&list, e);
                }
            }
            break;
        }
        default: {
            printf("Неверно. Нужно было вводить правильные чиселки  ;(");
            return 0;
        }
    }
    printf("\n\nFOREACH:\nВыведем список через перевод строки:\n");
    void (*pf)(int) = &fun_print_with_n;
    foreach(list, pf);
    printf("\nИ через пробел:\n");
    pf = &fun_print_with_space;
    foreach(list, pf);

    printf("\n\nMAP:\nВозведем каждый элемент списка в квадрат:\n");
    int (*ipf)(int) = &fun_square;
    struct list* res_list = map(list, ipf);
    foreach(res_list, pf);

    printf("\nВозведем каждый элемент списка в куб:\n");
    ipf = &fun_cube;
    res_list = map(list, ipf);
    foreach(res_list, pf);
    list_free(&res_list);

    int (*ipfii)(int, int) = &fun_sum;
    printf("\n\nFOLDL:\nСумма элементов, посчитанная с помощью foldl: %d\n", foldl(0, ipfii, list));
    ipfii = &fun_max;
    printf("Максимальный элемент в list: %d\n", foldl(INT_MIN, ipfii, list));
    ipfii = &fun_min;
    printf("Минимальный элемент в list: %d\n", foldl(INT_MAX, ipfii, list));

    printf("\nMAP_MUT:\nМодуль всех чисел:\n");
    int (*ipfi)(int) = &fun_abs;
    map_mut(list, ipfi);
    foreach(list, pf);

    printf("\n\nITERATOR:\n10 первых степеней двойки:\n");
    ipfi = &fun_pow_two;
    res_list = iterator(1, 10, ipfi);
    foreach(res_list, pf);
    list_free(&res_list);


    save(list, "output.txt");
    printf("\n\nКонечный список сохранен в 'output.txt'\n");
    printf("\nПамять очищена.\n");
    list_free(&list);
    return 0;
}
