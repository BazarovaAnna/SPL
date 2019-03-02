#ifndef LOW_LEVEL_PRIGRAMMING_LABS_BMP_FUNCTIONS_H
#define LOW_LEVEL_PRIGRAMMING_LABS_BMP_FUNCTIONS_H

#include <stdint.h>
#include <stdio.h>

/**
 *  Тип - файл бмп
 *  Это его заголовки
 */
typedef struct bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} __attribute__((packed)) bmp_header;

/**
 *  Тип - пиксель
 *  Мы сохраняем его параметры по палитре RGB 
 */
typedef struct pixel_t{
    uint8_t b,g,r;
} pixel_t;
/**
 *  Тип - картинка
 *  Хранятся ширина и высота
 *  А также массив из пикселей
 */
typedef struct image_t {
    uint32_t width, height;
    struct pixel_t* data;
} image_t;

/**
 *  Это состояния для считывания
 */
typedef enum {
    READ_OK,//считывание прошло успешно
    READ_INVALID_SIGNATURE,//и т.д. копипаста  из учебника
    READ_INVALID_BITS,
    READ_INVALID_HEADER
} read_status;

/**
 *  Это состояния для записи
 */
typedef enum {
    WRITE_OK,//успешно
    WRITE_ERROR//догадайтесь сами
} write_status;

/**
 *  Функция для считывания данных из файла - картинки
 *  @param in - файл из которого берем данные
 *  @param read - наша картинка, но уже как данные
 *  @return статус чтения (READ_OK если все хорошо)
 */
read_status from_bmp(FILE* in, image_t* const read);

/**
 *  Функция для записи данных в файл
 *  @param out - файл, в который записываем данные
 *  @param img - наша картинка, как данные
 *  @return статус записи (WRITE_OK если все хорошо)
 */
write_status to_bmp(FILE* out, image_t const* img );

/**
 *  Функция для поворачивания картинки вправо на 90 градусов
 *  @param image - исходная картинка
 *  @return новая картинка, которая по сути есть повернутая картинка из параметров
 */
image_t rotate_right_90(image_t const image);

/**
 *  Функция для поворачивания картинки влево на 90 градусов
 *  @param image - исходная картинка
 *  @return новая картинка, которая по сути есть повернутая картинка из параметров
 */
image_t rotate_left_90(const image_t image);
#endif //LOW_LEVEL_PRIGRAMMING_LABS_BMP_FUNCTIONS_H
