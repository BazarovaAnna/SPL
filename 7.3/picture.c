#include <stdlib.h>

#include <bits/types/FILE.h>

#include <stdio.h>

#include "picture.h"



write_error_code_t write_bmp(char const* filename, image_t const* image){



    if (image == NULL){

        return WRITE_IMAGE_NOT_FOUND;

    }

    if (filename == NULL){

        return WRITE_FILENAME_NOT_FOUND;

    }

    bmp_header_t* header = (bmp_header_t*)malloc(sizeof(bmp_header_t));

    int padding = image->width % 4;

    uint32_t i, j;

    image_t* new_image = (image_t*)malloc(sizeof(image_t));

    new_image->width = image->width+padding;

    new_image->height = image->height;

    new_image->data = (pixel_t*)calloc(1, new_image->height * new_image->width * sizeof(pixel_t));



    for (i = 0; i < new_image->height; ++i){

        for (j = 0; j < new_image->width; ++j){

            if(j < new_image->width - padding) {

                *(new_image->data + i * new_image->width + j) = *(image->data + i * image->width + j);

            }

        }

    }



    FILE* output = fopen(filename, "wb+");

    if (output == NULL){

        return WRITE_ERROR;

    }

    header->bfType = 19778;

    header->bfileSize = new_image->width * new_image->height * sizeof(pixel_t) + sizeof(header);

    header->bfReserved = 0;

    header->bOffBits = sizeof(bmp_header_t);

    header->biSize = 40;

    header->biPlanes = 0;

    header->biBitCount = 24;

    header->biCompression = 0;

    header->biSizeImage = new_image->width * new_image->height * sizeof(pixel_t);

    header->biXPelsPerMeter = 2835;

    header->biYPelsPerMeter = 2835;

    header->biClrUsed = 0;

    header->biClrImportant = 0;

    header->biWidth = new_image->width - padding;

    header->biHeight = new_image->height;



    fwrite(header, 1, sizeof(bmp_header_t), output);

    fwrite(image->data, 1, new_image->height * new_image->width * sizeof(pixel_t), output);

    fclose(output);

    return WRITE_OK;

}



read_error_code_t read_bmp(char const* filename, image_t* input_image){



    if (filename == NULL){

        return READ_FILENAME_NOT_FOUND;

    }

    FILE* input = fopen(filename, "rb");

    if (input == NULL){

        return READ_FILE_NOT_FOUND;

    }

    bmp_header_t header;

    fread(&header, 1, sizeof(header), input);

    if (header.bfType == 0){

        return READ_INVALID_HEADER;

    }

    uint8_t* data = (uint8_t *)malloc(header.biSizeImage);

    fseek(input, header.bOffBits, SEEK_SET);

    fread(data, 1, header.biSizeImage, input);

    if (data == NULL){

        return READ_INVALID_BITS;

    }

    if (input_image == NULL) {

        input_image = (image_t*)malloc(sizeof(image_t));

    }

    input_image->data = (pixel_t*)malloc(header.biHeight * header.biWidth * sizeof(pixel_t));



    int padding = header.biWidth % 4;

    for (uint32_t i = 0; i < header.biHeight; ++i){

        for (uint32_t j = 0; j < header.biWidth; ++j){

            *(input_image->data + i*header.biWidth + j) = *(pixel_t*)(((uint8_t*)data) + sizeof(pixel_t)*(i*header.biWidth + j) + padding*i);

        }

    }

    input_image->height = header.biHeight;

    input_image->width = header.biWidth;

    fclose(input);

    return READ_OK;

}
