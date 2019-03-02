#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>//here lies mmap

#include <stdbool.h>
#define MIN_QUERY_SIZE 8
#define INIT_BLOCK_SIZE 4096

typedef struct block_header block_header;
char* start;
struct __attribute__((packed)) block_header {
	block_header* next;//следующий блок где
	size_t capacity;//размер
	bool is_free;//свободен ли
};
block_header* init() {
	start = mmap(start, INIT_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0);//откуда, длина, доступ на чтение и запись, доступ только одному процессу, дескриптор, отступ
	if (start == MAP_FAILED)
		start = mmap(NULL, INIT_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);//если с точки старта не удалось, поставим с null
	block_header* header = (block_header*)start;
	header->next = NULL;
	header->capacity = INIT_BLOCK_SIZE;
	header->is_free = true;
	return header;
}
void merge(block_header* header) {
	if (header->next != NULL && header->is_free == true && header->next->is_free == true) {//если оба соседних блока свободны то сделаем из них один
		header->capacity += header->next->capacity + sizeof(block_header);
		header->next = header->next->next;
	}
}
void* customalloc(size_t query) {
	if (query < MIN_QUERY_SIZE)//кладем в блок столько данных сколько надо
		query = MIN_QUERY_SIZE;
	block_header* header = (block_header*)start;
	merge(header);
	while (header->is_free == false || header->capacity < query) {
		merge(header);
		header = header->next;
		if (header == NULL) {
			header = init();
			break;
		}
	}
	header->is_free = false;
	// отдадим остаток места другому блоку, если этот блок достаточно большой
	if (header->capacity - query >= MIN_QUERY_SIZE + sizeof(block_header)) {
		block_header* new_header = (block_header*)((uint8_t*)header + query + sizeof(block_header));
		new_header->capacity = header->capacity - query - sizeof(block_header);
		header->capacity -= new_header->capacity;
		new_header->is_free = true;
		header->next = new_header;
	}
	return (void*)((uint8_t*)header + sizeof(block_header));
}
void customfree(void* ptr) {
	block_header* header = (block_header*)((uint64_t)ptr - sizeof(block_header));//указатель возвращаем назад
	header->is_free = true;//добби свободен
	merge(header);//сливаем с предыдущим
}
int main() {
	start = mmap(NULL, INIT_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	void* smth = customalloc(240);
	void* smth_else = customalloc(50);
	void* another_thing = customalloc(25);
	block_header* h = (block_header*)start;
	while (h != NULL) {
		printf("h_cap = %ld\n", h->capacity);
		h = h->next;
	}//показывает все выделенные нам участки памяти(их размер)
	customfree(smth);
	customfree(smth_else);
	customfree(another_thing);
	start = mmap(NULL, INIT_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	void* finel = customalloc(17);
	printf("-----------------------\n");
	h = (block_header*) start;
	while (h != NULL) {
		printf("h_cap = %ld\n", h->capacity);
		h = h->next;
	}
	customfree(finel);
	return 0;
}
