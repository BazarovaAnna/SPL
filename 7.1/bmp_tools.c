#include <stdlib.h>

#include <stdio.h>

#include "bmp_tools.h"



bmp_header* read_bmp_header(FILE* file) {

  bmp_header* header = (bmp_header*)malloc(sizeof(bmp_header));

  fread(header, sizeof(bmp_header), 1, file);

  return header;

}



read_status read_bmp_data(FILE* file, image_t* img) {

  /* load raw (padded) data from source image */

  bmp_header* header = read_bmp_header(file);

  if (header->bits_per_pixel != 24)

    return READ_ERR_BITS_PER_PIXEL;

  if (header->file_type[0] != 'B' || header->file_type[1] != 'M')

    return READ_ERR_SIGNATURE;

  uint8_t* raw = (uint8_t*)malloc(header->img_size);

  fseek(file, header->img_data_offset, SEEK_SET);

  if (fread(raw, header->img_size, 1, file) != 1)

    return READ_ERR;



  /* now its time to remove some padding folks */

  uint32_t pad = header->width % 4;    // this is padding - how many bytes we should add for each line

  if (pad == 0) {

    img->data = (pixel_t*) raw;

  }

  else {

    pixel_t* valid_data = (pixel_t*)malloc(header->width * header->height * sizeof(pixel_t));

    uint64_t acc_pad;                   // padding accumulator

    uint64_t cur_pixel;

    uint32_t h, w;

    for (h = 0; h < header->height; h++) {

      acc_pad = h * pad;

      for (w = 0; w < header->width; w++) {

        cur_pixel = h * header->width + w;

        valid_data[cur_pixel] = *(pixel_t*) (raw + cur_pixel * sizeof(pixel_t) + acc_pad);

      }

    }

    free(raw);

    img->data = valid_data;

  }

  img->width = header->width;

  img->height = header->height;

  fseek(file, 0, SEEK_SET);

  return READ_OK;

}



bmp_header* create_bmp_header(image_t* img) {

  bmp_header* header = (bmp_header*)malloc(sizeof(bmp_header));



  header->file_type[0] = 'B';

  header->file_type[1] = 'M';

  header->reserved_zero = 0;

  header->img_data_offset = sizeof(bmp_header);

  header->header_size = header->img_data_offset - 14;

  header->width = img->width;

  header->height = img->height;

  header->planes = 1;

  header->bits_per_pixel = 24;

  header->compression = 0;

  header->img_size = img->height * img->width * sizeof(pixel_t) 

    + img->height * (img->width % 4);

  header->pix_per_meter_hor = 2835;

  header->pix_per_meter_ver = 2835;

  header->number_of_colors = 0;

  header->number_of_important_colors = 0;

  header->file_size = header->img_data_offset + header->img_size;



  return header;

}



write_status write_bmp(image_t* img, FILE* file) {

  bmp_header* header = create_bmp_header(img);

  if (fwrite(header, sizeof(bmp_header), 1, file) != 1)

    return WRITE_ERR;

  

  uint64_t num_bytes = img->width * img->height * sizeof(pixel_t);



  uint64_t pad = img->width % 4;

  if (pad == 0) {

    if (fwrite(img->data, num_bytes, 1, file) != 1)

      return WRITE_ERR;

  }

  else {

    uint64_t data_size = img->width * img->height * sizeof(pixel_t) + img->height * pad;

    uint8_t* data = (uint8_t*)calloc(1, data_size);

    uint64_t h, w, cur_pad, cur_pix;

    for (h = 0; h < img->height; h++) {

      cur_pad = h * pad;

      for (w = 0; w < img->width; w++) {

        cur_pix = h * img->width + w;

        *((pixel_t*) (data + sizeof(pixel_t) * cur_pix + cur_pad)) = img->data[cur_pix];

      }

    }

    fwrite(data, data_size, 1, file);

    free(data);

  }

  fflush(file);

  return WRITE_OK;

}
