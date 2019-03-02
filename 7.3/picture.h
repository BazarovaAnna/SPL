#ifndef LAB5_PICTURE_H

#define LAB5_PICTURE_H



#include <stdint.h>



typedef struct __attribute__((packed)){

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

} bmp_header_t;



typedef struct {

    unsigned char b,g,r;

} pixel_t;



typedef struct {

    uint32_t width, height;

    pixel_t* data;

} image_t;



typedef enum {

    READ_OK = 0,

    READ_FILE_NOT_FOUND,

    READ_INVALID_BITS,

    READ_INVALID_HEADER,

    READ_FILENAME_NOT_FOUND

} read_error_code_t;



typedef enum {

    WRITE_OK = 0,

    WRITE_ERROR,

    WRITE_IMAGE_NOT_FOUND,

    WRITE_FILENAME_NOT_FOUND,

} write_error_code_t;



/**

 * Функция чтения байт с bmp-файла

 * @param filename - имя bmp-файла

 * @param new_image - для записи прочитанной картинки

 * @return   read_error_code_t

 */

read_error_code_t read_bmp(char const* filename, image_t* new_image);



/**

 * Функция записи изображение в bmp-файл

 * @param filename - имя файла

 * @param image - картинка

 * @return write_error_code_t

 */

write_error_code_t write_bmp(char const* filename, image_t const* image);



#endif //LAB5_PICTURE_H
