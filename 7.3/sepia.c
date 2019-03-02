#include "sepia.h"



#include <inttypes.h>

#include <glob.h>

#include <stdio.h>



static unsigned char sat( uint64_t x) {

    if (x < 256)

        return (unsigned char)x;

    return 255;

}



static void sepia_one(pixel_t* const pixel ) {

    static const float c[3][3] =  {

            { .393f, .769f, .189f },

            { .349f, .686f, .168f },

            { .272f, .543f, .131f } };

    pixel_t const old = *pixel;

    pixel->r = sat( (uint64_t) (old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2]));

    pixel->g = sat( (uint64_t)(old.r * c[1][0] + old.g * c[1][1] + old.b  * c[1][2]));

    pixel->b = sat( (uint64_t) (old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2]));

}



void sepia_c_inplace(image_t* pic) {

    for (size_t i = 0; i < pic->height; i++) {

        for (size_t j = 0; j < pic->width; j++) {

            sepia_one(pic->data + i*pic->width + j);

        }

    }

}



void image_sepia_sse(pixel_t* pixel, uint32_t size);



void sepia_sse_inplace(image_t *img) {
	printf( "Вошли");

    if (img->height * img->width < 4){

        for (int i = 0; i < img->height * img->width; ++i){

            sepia_one(img->data + i);

        }

        return;

    }

    image_sepia_sse(img->data, img->height * img->width - (img->height * img->width) % 4);

    for (int i  = img->height * img->width - (img->height * img->width) % 4; i < img->height * img->width; ++i){

        sepia_one(img->data + i);

    }

}

