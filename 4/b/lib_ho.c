#include <stdio.h>
#include <stdlib.h>
#include "lib_ho.h"

void foreach(struct list const* list, void (*f)(int)){
    while (list != NULL){
        f(list->elem);//пока не дойдем до первого, применяем к каждому значению ноды нужную функцию
        list = list->last;
    }
}

struct list* map(struct list const* list, int (*f)(int)){
    struct list* result_list = NULL;
    while (list != NULL){//пока не дойдем до первого
        list_add_front(&result_list, f(list->elem));//дописываем в новый список в начало новый элемент, полученный из старого применением функции
        list = list->last;
    }
    return result_list;
}

int foldl(int acc, int (*f)(int, int), struct list const* list){
    while (list != NULL){//для всех эл-тов списка
        acc = f(acc, list->elem);//мы для acc используем функцию от acc и элемента списка	
        list = list->last;
    }
    return acc;
}

void map_mut(struct list* list, int (*f)(int)){
    while(list != NULL){//для каждого элемента списка мы присваиваем значение функции от значения этого элемента
        list->elem = f(list->elem);
        list = list->last;
    }
}

struct list* iterator(int s, size_t n, int (*f)(int)){
    struct list* list = NULL;
    size_t i = 0;
    for(; i < n; ++i){
        list_add_front(&list, s);//[s,f(s),f(f(s)),f(f(f(s))),...] 
        s = f(s);
    }
    return list;
}
