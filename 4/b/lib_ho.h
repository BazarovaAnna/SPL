#include "lib_ll.h"

#ifndef LAB4_HIGHER_ORDER_H
#define LAB4_HIGHER_ORDER_H

/**
 *  функция по очереди выполняет функцию на всех элементах саиска по порядку
 *  @param list - указатель на list элементов
 *  @param f - указатель на функцию
 */
void foreach(struct list const* list, void (*f)(int));



/**
 * Функция создает новый список, в котором к содержимомоу каждому элементу list'а применяется функция f
 * @param list - указатель на исходный список (список не изменяется)
 * @param f - указатель на функцию
 * @return новый list с результатом приминения f к каждому элементу list'a
 */
struct list* map(struct list const* list, int (*f)(int));



/**
 * Функция возвращает сущность того же типа, что и аккумулятор, запуская f т копии аккумулятора и текущим элементом списка
 * @param acc - начальное значение аккумулятора
 * @param f - указатель на функцию
 * @param list - список элементов
 * @return полученное значение
 */
int foldl(int acc, int (*f)(int, int), struct list const* list);



/**
 * Функция изменяет каждый элемент list'a, применяя функцию f
 * @param list
 * @param f
 */
void map_mut(struct list* list, int (*f)(int));



/**
 * Ренерирует списов вида: [s, f(s), f(f(s)), ...], длинной n
 * @param s - первый элемент списка
 * @param n - длинна списка
 * @param f - указатель на функцию
 * @return новый список
 */
struct list* iterator(int s, size_t n, int (*f)(int));
#endif //LAB4_HIGHER_ORDER_H
