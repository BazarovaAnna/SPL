#include <stdio.h>

#include "picture.h"

#include "sepia.h"

#include <stdlib.h>

#include <sys/time.h>

#include <sys/resource.h>

#include <unistd.h>



void write_ans(char* filename, image_t* img){

    switch (write_bmp(filename, img)){

        case WRITE_IMAGE_NOT_FOUND: {

            printf("Изображение для записи не найдено.\n");

            break;

        }

        case WRITE_FILENAME_NOT_FOUND: {

            printf("А тут как-то неправильно передается filename.\n");

            break;

        }

        case WRITE_ERROR: {

            printf("Незнаю, как такое получилось, но тут ошибка при открытии файла.\n");

            break;

        }

        case WRITE_OK: {

            printf("Я записаль в %s :3\n", filename);

            break;

        }

        default: {

            printf("Совсем что-то не так пошло.\n");

            break;

        }

    }

}





int main() {

    image_t image_c;



    switch (read_bmp("s.bmp", &image_c)) {

        case READ_FILENAME_NOT_FOUND :{

            printf("Не найден файл.\n");

            return 1;

        }

        case READ_INVALID_BITS: {

            printf("Проблемы с данными.\n");

            return 1;

        }

        case READ_INVALID_HEADER: {

            printf("Проблемы с заголовком.\n");

            return 1;

        }

        case READ_OK:{

            printf("Изображение получено.\n");

            break;

        }

        default: {

            printf("Вообще что-то не так.\n");

            return 1;

        }

    }



    image_t image_asm;

    read_bmp("s.bmp", &image_asm);



    struct rusage r;

    struct timeval start;

    struct timeval end;

    getrusage(RUSAGE_SELF, &r);

    start = r.ru_utime;



    sepia_c_inplace(&image_c);



    getrusage(RUSAGE_SELF, &r);

    end = r.ru_utime;

    long res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;

    printf( "Время выполнения sepia (с): %ld\n", res);

    printf( "end.tv_sec: %ld\n", end.tv_sec);

    printf( "start.tv_sec: %ld\n", start.tv_sec);

    printf( "end.tv_usec: %ld\n", end.tv_usec);

    printf( "start.tv_usec: %ld\n", start.tv_usec);

    write_ans("out_c.bmp", &image_c);



    getrusage(RUSAGE_SELF, &r);

    start = r.ru_utime;

    sepia_sse_inplace(&image_c);



    getrusage(RUSAGE_SELF, &r);

    end = r.ru_utime;

    res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;

    printf( "Время выполнения sepia (sse): %ld\n", res);

    printf( "end.tv_sec: %ld\n", end.tv_sec);

    printf( "start.tv_sec: %ld\n", start.tv_sec);

    printf( "end.tv_usec: %ld\n", end.tv_usec);

    printf( "start.tv_usec: %ld\n", start.tv_usec);



    write_ans("out_sse.bmp", &image_asm);



    return 0;

}
