#include <stdlib.h>
#include <stdio.h>
#include "bmp_functions.h"

read_status from_bmp(FILE* in, image_t* const read){
    bmp_header header;

    fread(&header, sizeof(header), 1, in);//считали заголовки бмп, молодцы


    if(header.bfType!=19778){
        return READ_INVALID_SIGNATURE;
    }
    if(header.biBitCount !=24){//по заданию глубина цвета должна быть ровно 24
        return READ_INVALID_BITS;
    }

    read->width = header.biWidth;//сохраняем только те из заголовков бмп, что нам понадобятся
    read->height = header.biHeight;

    read->data = malloc(read->width * read->height * sizeof(pixel_t));//выделяем столько памяти, сколько пикселей * на размер пикселя

    int padding = (4 - (read->width * sizeof(pixel_t))%4)%4;//это смещение

    pixel_t *data = read->data;

    for (int i = 0; i < read->height; ++i) {
        for (int j = 0; j < read->width; ++j) {
            fread(data, sizeof(pixel_t), 1, in);//попиксельно считали картинку, ура
            data++;
        }
        fseek(in, padding, SEEK_CUR);//"перепрыгиваем" мусор
    }

    return READ_OK;

}

write_status to_bmp(FILE* out, image_t const* img) {
    bmp_header header;//расставляем все заголовки
    header.bfType = 19778;
    header.biWidth = abs(img->width);
    header.biHeight = img->height;
    int padding = (4 - (header.biWidth * sizeof(pixel_t)) % 4) % 4;
    header.bOffBits = 54;
    header.biBitCount = 24;
    header.biSizeImage = abs(header.biHeight) * header.biWidth * sizeof(pixel_t) + abs(header.biHeight)*padding;
    header.bfileSize = header.bOffBits + header.biSizeImage;
    header.bfReserved = 0;
    header.biCompression = 0;
    header.biSize = 40;
    header.biPlanes = 1;
    header.biClrUsed = 0;
    header.biClrImportant = 0;
    header.biXPelsPerMeter = 2834;
    header.biYPelsPerMeter = 2834;
    fwrite(&header,sizeof(bmp_header),1,out);
    for (int i = 0; i < abs(header.biHeight); i++) {
        for (int j = 0; j < header.biWidth; j++) {
            pixel_t triple;
            triple = img->data[header.biWidth*i + j];//попиксельная запись в файл
            fwrite(&triple,sizeof(pixel_t),1,out);
        }
        for (int j = 0; j < padding; j++) {
            fputc(0x00, out);//заполняем остаток "мусором"
        }
    }
    return WRITE_OK;
}


image_t rotate_left_90(const image_t image) {
    image_t *rotated = malloc(sizeof(image_t));
    rotated->width = image.height;
    rotated->height = image.width;
    pixel_t *rotate = malloc(abs(rotated->width) * rotated->height * sizeof(pixel_t));

    for (int i = 0; i < abs(image.height); ++i) {
        for (int j = 0; j < image.width; ++j) {
            pixel_t temp = image.data[image.width * i + j];
            rotate[abs(rotated->width) * (j + 1) - i - 1] = temp;
        }
    }
    rotated->data = rotate;
    return *rotated;
}

image_t rotate_right_90(const image_t image) {
    image_t *rotated = malloc(sizeof(image_t));
    rotated->width = image.height;
    rotated->height = image.width;
    pixel_t *rotate = malloc(abs(rotated->width) * rotated->height * sizeof(pixel_t));

    for (int i = 0; i < abs(image.height); ++i) {
        for (int j = 0; j < image.width; ++j) {
            pixel_t temp = image.data[image.width * i + j];
            rotate[abs(rotated->width) * (rotated->height -j -1) + i  ] = temp;
        }
    }
    rotated->data = rotate;
    return *rotated;
}
