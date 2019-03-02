#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/**
 *  Структура связного списка
 *  Элементом списка является int
 *  Хранятся указатели на следующий и предыдущий элемент
 */
struct list {
    int elem;
    struct list* next;
    struct list* last;
};
//задумайся, человек! list* - это всего лишь последняя нода нашего великого связного списка
//Зная хоть один list* мы знаем весь связный список ибо этого достаточно, поэтому везде передаем ее, о человек!
/**
 *  Функция для создания списка с первым элементом
 *  @param e - элемент списка
 *  @return указатель на список
 */
struct list* list_create(int e);

/**
 *  Функция для добавления элемента в начало списка
 *  Если список пуст, создает новый список
 *  @param list - указатель на список, в который добавляем элемент
 */
void list_add_front(struct list** list, int e);

/**
 *  Функция для добавления элемента в конец списка
 *  Если список пуст, создает новый список
 *  @param list - указатель на список, в который добавляем элеент
 */
void list_add_back(struct list** list, int e);

/**
*  Функция возвращает значение элемента списка по индексу
*  @param list - указатель на список
*  @param index - индекс элемента списка
*  @return значение элемента списка (если индекса в списке нет, вернет 0, шо поделать ¯\_(ツ)_/¯)
*/
int list_get(struct list const* list, unsigned int index);

/**
 *  Функция удаляет все элементы списка
 *  @param list - указатель на список
 */
void list_free(struct list** list);

/**
 *  Функция считает длинну списка
 *  @param  list - указатель на список
 *  @return длинну списка
 */
size_t list_length(struct list const* list);

/**
*  Функция возвращает элемент списка по индексу
*  @param list - указатель на список
*  @param index - индекс элемента списка
*  @return элемент списка или NULL, если индекс выходит за границы списка или list пуст
*/
struct list* list_node_at(struct list const* list, unsigned int index);

/**
 *  Функция считает сумму элементов списка
 *  @param list - указатель на list
 *  @return сумму элементов списка
 *          0, если list пуст (в общем-то, логично)
 */
int list_sum(struct list const* list);

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

int main(int argc, char** argv){	struct list* list = NULL;
	int e;
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
    
    printf("\n\nСумма всех элементов списка: \n%d", list_sum(list));
    
    if(list_node_at(list,3)==NULL){
		printf("\n\nЭлемента #3 нет, так как длина списка всего: %ld",list_length(list));
	}else{
		printf("\n\nЭлемент #3:\n%d", list_get(list, 3));
	}
    save(list, "output.txt");
    printf("\n\nКонечный список сохранен в 'output.txt'\n");

    printf("\nПамять очищена.\n");
    list_free(&list);
	return 0;
}

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
/*
 * list_create+
 * list_add_front+
 * list_add_back+
 * list_get+
 * list_free+
 * list_length+
 * list_node_at+
 * list_sum+
 */
 
