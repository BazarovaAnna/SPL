#include <stdint.h>

#include <stdio.h>

#include <stdlib.h>

#include "bmp_tools.h"

#include "sepia.h"



#include <sys/time.h>

#include <sys/resource.h>

#include <unistd.h>



#define exit_with_err(...) do { fprintf(stderr, __VA_ARGS__); return 1; } while (0);

extern void image_sepia_sse(pixel_t* pixel, uint64_t size);

extern void image_sepia_avx(pixel_t* pixel, uint64_t size);



#define BENCHMARK_NUM_RUNS 250UL





void sepia_avx_inplace(image_t* img) {

  uint64_t num_pixels = img->height * img->width;



  if (num_pixels < 8) { for (int p = 0; p < num_pixels; ++p) sepia_one(img->data + p); return; }



  image_sepia_avx(img->data, num_pixels - num_pixels % 8);



  for (int p = num_pixels - num_pixels % 8; p < num_pixels; ++p) sepia_one(img->data + p);

}





void sepia_sse_inplace(image_t *img) {

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





void calculate_time(image_t* img) {

  struct rusage r;

  struct timeval start, end;

  

  getrusage(RUSAGE_SELF, &r); start = r.ru_utime;



  for (uint64_t i = 0; i < BENCHMARK_NUM_RUNS; i++) sepia_avx_inplace(img);



  getrusage(RUSAGE_SELF, &r); end = r.ru_utime;



  long res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;



  printf("AVX: %ld μs (average for %lu runs)\n", res / BENCHMARK_NUM_RUNS, BENCHMARK_NUM_RUNS);





  getrusage(RUSAGE_SELF, &r); start = r.ru_utime;



  for (uint64_t i = 0; i < BENCHMARK_NUM_RUNS; i++) { sepia_sse_inplace(img); }



  getrusage(RUSAGE_SELF, &r); end = r.ru_utime;



  res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;



  printf("SSE: %ld μs (average for %lu runs)\n", res / BENCHMARK_NUM_RUNS, BENCHMARK_NUM_RUNS);



  // ---



  getrusage(RUSAGE_SELF, &r); start = r.ru_utime;



  for (uint64_t i = 0; i < BENCHMARK_NUM_RUNS; i++) { sepia_c_inplace(img); }



  getrusage(RUSAGE_SELF, &r); end = r.ru_utime;



  res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;



  printf("Native C: %ld μs (average for %lu runs)\n", res / BENCHMARK_NUM_RUNS, BENCHMARK_NUM_RUNS);

}



int main(int argc, char** argv) {

  if (argc != 2)

    exit_with_err("usage: lab7 src_file.bmp\n");



  char* input_name = argv[1];



  FILE* input = fopen(input_name, "rb");

  if (input == NULL)

    exit_with_err("Cannot open %s\n", input_name);

  FILE* output_c = fopen("out/result_c.bmp", "wb");

  FILE* output_sse = fopen("out/result_sse.bmp", "wb");

  FILE* output_avx = fopen("out/result_avx.bmp", "wb");

  image_t* img = (image_t*) malloc(sizeof(image_t));

  read_status r_st = read_bmp_data(input, img);

  image_t* img_calc = (image_t*)malloc(sizeof(image_t));

  r_st = read_bmp_data(input, img_calc);

  calculate_time(img_calc);



  switch (r_st) {

    case READ_ERR_SIGNATURE:

      exit_with_err("%s is not a valid bmp file\n", input_name);

    case READ_ERR_BITS_PER_PIXEL:

      exit_with_err("Only 24-bit images supported\n");

    case READ_ERR:

      exit_with_err("Error ocured while opening %s\n", input_name);

    case READ_OK:

      break;

  }

  

  image_t* img_c = (image_t*)malloc(sizeof(image_t));

  r_st = read_bmp_data(input, img_c);

  sepia_c_inplace(img_c);

  write_bmp(img_c, output_c);



  sepia_sse_inplace(img);

  write_bmp(img, output_sse);



  image_t* img_avx = (image_t*)malloc(sizeof(image_t));

  r_st = read_bmp_data(input, img_avx);

  sepia_avx_inplace(img_avx);

  write_bmp(img_avx, output_avx);



  fclose(input);

  fclose(output_c);

  fclose(output_sse);

  fclose(output_avx);



  return 0;

}
