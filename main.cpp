#include <iostream>
#include <png.h>
#include <array>
#include <vector>

using namespace std;

int main() {
  // create 28x28 array of 0s
  array<array<int, 28>, 28> arr;

  // write it as a png image
  FILE *fp = fopen("test.png", "wb");
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info_ptr = png_create_info_struct(png_ptr);
  png_init_io(png_ptr, fp);
  png_set_IHDR(png_ptr, info_ptr, 28, 28, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png_ptr, info_ptr);
  png_bytep row = (png_bytep) malloc(28 * sizeof(png_byte));
  for (int y = 0; y < 28; y++) {
    for (int x = 0; x < 28; x++) {
      row[x] = arr[y][x];
    }
    png_write_row(png_ptr, row);
  }
  png_write_end(png_ptr, NULL);
  fclose(fp);

  return 0;
}