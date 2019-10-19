
#include <stdio.h>
#include <stdlib.h>

#include "libfar.h"

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

char* read_pcm(const char* filename, size_t* size) {
  FILE* fp = fopen(filename, "rb");
  if (!fp) {
    return 0;
  }
  fseek(fp, 0, SEEK_END);
  *size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  size_t aligned_size = (*size + 255) & ~(size_t)0xFF;
  char* buffer = (char*)far_malloc_align(aligned_size, 256);
  if (!buffer) {
    return 0;
  }
  fread(buffer, 1, *size, fp);
  fclose(fp);
  return buffer;
}

void write_pcm(const char* buffer, size_t size, const char* filename) {
  FILE* fp = fopen(filename, "wb");
  if (!fp) {
    return;
  }
  fwrite(buffer, 1, size, fp);
  fclose(fp);
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    return -1;
  }
  size_t size_in;
  char* buf_in = read_pcm(argv[1], &size_in);
  if (!buf_in) {
    return -2;
  }
  size_t size_out = size_in * 2;
  size_t aligned_size_out = (size_out + 255) & ~(size_t)0xFF;
  char* buf_out = (char*)far_malloc_align(aligned_size_out, 256);
  if (!buf_out) {
    free(buf_in);
    return -3;
  }
  for (int k = 0; k < 10; ++k) {
    LARGE_INTEGER s, e, freq;
    QueryPerformanceCounter(&s);
    for (int i = 0; i < 100; ++i) {
      far_downsample2f32(buf_in, size_in, 44100, 2, buf_out, aligned_size_out);
    }
    QueryPerformanceCounter(&e);
    QueryPerformanceFrequency(&freq);
    LONGLONG delta = (e.QuadPart - s.QuadPart);
    double dt = (double)delta / freq.QuadPart;
    fprintf(stderr, "delta=%d dt=%f\n", (int)delta, dt);
  }
  far_free_align(buf_in);
  write_pcm(buf_out, size_out, argv[2]);
  far_free_align(buf_out);
  return 0;
}
