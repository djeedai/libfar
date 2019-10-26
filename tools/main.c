
#include <stdio.h>
#include <stdlib.h>

#include "libfar.h"

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#ifdef __cplusplus
using namespace libfar;
#endif

byte32* read_pcm(const char* filename, size_t* size) {
  FILE* fp = fopen(filename, "rb");
  if (!fp) {
    return 0;
  }
  fseek(fp, 0, SEEK_END);
  *size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  size_t aligned_size = (*size + 31) & ~(size_t)0x1F;
  byte32* buffer = FAR_FN(malloc_align32)(aligned_size);
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

int run_tools(int argc, char* argv[]) {
  if (argc < 3) {
    return -1;
  }
  size_t size_in;
  byte32* buf_in = read_pcm(argv[1], &size_in);
  if (!buf_in) {
    return -2;
  }
  size_t size_out = size_in * 2;
  size_t aligned_size_out = (size_out + 31) & ~(size_t)0x1F;
  byte32* buf_out = FAR_FN(malloc_align32)(aligned_size_out);
  if (!buf_out) {
    free(buf_in);
    return -3;
  }
  for (int k = 0; k < 10; ++k) {
    LARGE_INTEGER s, e, freq;
    QueryPerformanceCounter(&s);
    for (int i = 0; i < 100; ++i) {
      FAR_FN(downsample2f32)(buf_in, size_in, 44100, 2, buf_out, aligned_size_out);
    }
    QueryPerformanceCounter(&e);
    QueryPerformanceFrequency(&freq);
    LONGLONG delta = (e.QuadPart - s.QuadPart);
    double dt = (double)delta / freq.QuadPart;
    fprintf(stderr, "delta=%d dt=%f\n", (int)delta, dt);
  }
  FAR_FN(free_align32)(buf_in);
  write_pcm((const char*)buf_out, size_out, argv[2]);
  FAR_FN(free_align32)(buf_out);
  return 0;
}
