#include <inttypes.h>

#include "bmp_tools.h"





static unsigned char sat( uint64_t x) {

    if (x < 256) return x;

    return 255;

}



void sepia_one( pixel_t* pixel ) {

  static const float c[3][3] =  {

  { .393f, .769f, .189f },

  { .349f, .686f, .168f },

  { .272f, .543f, .131f } };

pixel_t old = *pixel;

pixel->r = sat(

        old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2]

        );

pixel->g = sat(

        old.r * c[1][0] + old.g * c[1][1] + old.b  * c[1][2]

        );

pixel->b = sat(

        old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2]

        );

}

void sepia_c_inplace( image_t* img ) {

    uint32_t h, w;

    for( h = 0; h < img->height; ++h )

        for( w = 0; w < img->width; ++w ) {

            pixel_t* p =  img->data + h * img->width + w;

            sepia_one(p);

        }

}
