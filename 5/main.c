#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_functions.h"
int main(int argc, char *argv[]){

   //будем передавать в аргументах командной строки файл откуда, затем файл куда, затем right или left

    FILE *file = fopen(argv[1], "rb");//откуда

    FILE *outptr = fopen(argv[2], "wb");//куда

    if(!file){
        printf("Ошибка! Файл не найден, проверьте правильность введенных аргументов\n");
        return 2;
    }


    image_t *image = malloc(sizeof(image_t));//выделяем память под новую картинку

    read_status result = from_bmp(file, image); //в файле с бмп функциями

    switch (result){
            case READ_INVALID_HEADER: {
                printf("Ошибка! Косяки в заголовках!\n");
                return 2;
            }
            case READ_INVALID_BITS: {
                printf("Ошибка! Проблемы с глубиной цвета картинки\n");
                return 2;
            }
            case READ_INVALID_SIGNATURE:{
				printf("Ошибка! Проблемы с сигнатурой!\n");
				return 2;
			}
            case READ_OK:{
                printf("Считывание произошло успешно!\n");
                break;
            }
        }

        image_t rotated;//это наша новая картиночка
        if(strcmp(argv[3], "left") == 0){
            rotated = rotate_left_90(*image);//в файле с бмп функциями
            printf("Совершаем поворот налево...\n");
        } else if(strcmp(argv[3], "right") == 0){
            printf("Совершаем поворот направо...\n");
            rotated = rotate_right_90(*image);//в файле с бмп функциями
        } else{
            printf("Ошибка! Вы не выбрали напрваление поворота!\n");
            return 2;
        }



    write_status result_write = to_bmp(outptr,&rotated);//в файле с бмп функциями

    if(result_write != WRITE_OK){
        printf("Ошибка с записью в файл!\n");
    } else printf("Запись в файл произошла успешно.\n");

    free(rotated.data);
}
