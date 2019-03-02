#include <stdlib.h>
#include <stdio.h>
#include "lib_ll.h"

struct list * list_create(int e) {
    struct list *new_elem_list = (struct list*)malloc(sizeof(struct list));//динамически выделяем память под первую ноду нашего списка (стд)
    new_elem_list->elem = e;//нынешний элемент, т.е. первый - e
    new_elem_list->last = NULL;//других элемнентов нет ни до ни после
    new_elem_list->next = NULL;
    return new_elem_list;
}

void list_add_front(struct list* *list, int e){
    if (*list == NULL){//если такого списка нет - создадим
        *list = list_create(e);
        return;
    }
    struct list *new_elem_list = (struct list*)malloc(sizeof(struct list));//выделим еще памяти под новую ноду
    new_elem_list->elem = e;
    new_elem_list->last = NULL;//так как дописали вперед, то перед ним ничего нет
    struct list* tmp_list = *list;
    while(tmp_list->last != NULL){
        tmp_list = tmp_list->last;//tmp_list это первый элемент *list
    }
    new_elem_list->next = tmp_list;//говорим нашей ноде что ее следующий элемент - это первый элемент *list
    tmp_list->last = new_elem_list;//говорим нашему первому элементу что его предыдущий элемент наша нода
    
}

void list_add_back(struct list** list, int e){
    if (*list == NULL){//если такого списка нет - создадим
        *list = list_create(e);
        return;
    }
    struct list *new_elem_list = (struct list*)malloc(sizeof(struct list));//выделим еще памяти под новую ноду
    new_elem_list->elem = e;
    new_elem_list->next = NULL;//так как дописали назад, то после него ничего нет
    new_elem_list->last = *list;
    (*list)->next = new_elem_list;//последний элемент теперь знает что он последний
    *list = new_elem_list;//теперь е - последняя нода, муа ха ха
}

int list_get(struct list const* list, unsigned int index){
    struct list* ret = list_node_at(list, index);//ret - либо указатель на ноду с нужным индексом, либо null если индекс выходит за пределы списка
    return ret == NULL ? 0 : ret->elem;
}

void list_free(struct list** list){
    while (*list != NULL) {//пока список не пуст, т.е. пока последняя нода есть
        struct list* last = (*list)->last;//запоминаем предыдущую ноду, а текущую "сжигаем"
        free(*list);//освобождаем ранее выделенную динамичесую память(стд)
        (*list) = last;//переходим к предыдущей ноде
    }
}

size_t list_length(struct list const* list){
    size_t i = 0;
    while (list != NULL) {
        struct list* last = list->last;//доходим с конца до первого элемента
        list = last;
        ++i;//параллельно считаем, сколько нод уже прошли
    }
    return i;
}

struct list* list_node_at(struct list const* list, unsigned int index){
    unsigned int i = 0;
    if (list == NULL){
        return NULL;//тут все логично
    }
    while (list->last != NULL){
        list = list->last;//находим первый элемент
    }
    while (i != index){
        ++i;
        list = list->next;
        if (list == NULL) {
            return NULL;//если следующего элемента нет, то index за пределами списка
        }
    }
    return list;//возвращаем нужную ноду
}

int list_sum(struct list const* list){
    int sum = 0;
    while (list != NULL){//пока не дошли до первого
        sum += list->elem;//суммируем значение
        list = list->last;//переходим к предыдущему
    }
    return sum;
}

bool load(struct list** list, const char* filename){
    int e;
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        return false;
    }
    while ( fscanf(input_file, "%d", &e) != EOF) {
        list_add_back(list, e);
    }
    fclose(input_file);
    return true;
}

bool save(struct list* list, const char* filename){
    FILE* output_file = fopen(filename, "w+");
    if (output_file == NULL){
        return false;
    }
    while (list != NULL) {
        fprintf(output_file, "%d ", list->elem);
        list = list->last;
    }
    fclose(output_file);
    return true;
}
