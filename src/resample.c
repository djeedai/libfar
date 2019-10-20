
#include "libfar.h"

extern void FAR_FN(upsample2f32_sse41)(const char* FAR_RESTRICT buf_in,
                                       size_t size_in,
                                       int sample_rate,
                                       int num_channels,
                                       char* FAR_RESTRICT buf_out,
                                       size_t size_out);
extern void FAR_FN(upsample3f32_sse41)(const char* FAR_RESTRICT buf_in,
                                       size_t size_in,
                                       int sample_rate,
                                       int num_channels,
                                       char* FAR_RESTRICT buf_out,
                                       size_t size_out);
extern void FAR_FN(downsample2f32_sse41)(const char* FAR_RESTRICT buf_in,
                                         size_t size_in,
                                         int sample_rate,
                                         int num_channels,
                                         char* FAR_RESTRICT buf_out,
                                         size_t size_out);
extern void FAR_FN(downsample3f32_sse41)(const char* FAR_RESTRICT buf_in,
                                         size_t size_in,
                                         int sample_rate,
                                         int num_channels,
                                         char* FAR_RESTRICT buf_out,
                                         size_t size_out);

extern void FAR_FN(upsample2f32_fpu)(const char* FAR_RESTRICT buf_in,
                                     size_t size_in,
                                     int sample_rate,
                                     int num_channels,
                                     char* FAR_RESTRICT buf_out,
                                     size_t size_out);
extern void FAR_FN(upsample3f32_fpu)(const char* FAR_RESTRICT buf_in,
                                     size_t size_in,
                                     int sample_rate,
                                     int num_channels,
                                     char* FAR_RESTRICT buf_out,
                                     size_t size_out);
extern void FAR_FN(downsample2f32_fpu)(const char* FAR_RESTRICT buf_in,
                                       size_t size_in,
                                       int sample_rate,
                                       int num_channels,
                                       char* FAR_RESTRICT buf_out,
                                       size_t size_out);

int g_cpuIdInitialized = 0;
int g_hasSSE = 0;
int g_hasSSE2 = 0;
int g_hasSSE41 = 0;
int g_hasSSE42 = 0;

void initCpuId() {
  int cpuinfo[4];
  __cpuid(cpuinfo, 0);
  int numIds = cpuinfo[0];
  int ecx1 = 0, edx1 = 0;
  if (numIds >= 1) {
    __cpuid(cpuinfo, 1);
    ecx1 = cpuinfo[2];
    edx1 = cpuinfo[3];
  }
  int ecx7 = 0, edx7 = 0;
  if (numIds >= 7) {
    __cpuid(cpuinfo, 7);
    ecx7 = cpuinfo[2];
    edx7 = cpuinfo[3];
  }
  g_hasSSE = (edx1 & 0x02000000);
  g_hasSSE2 = (edx1 & 0x04000000);
  g_hasSSE41 = (ecx1 & 0x00080000);
  g_hasSSE42 = (ecx1 & 0x00100000);
  // Support for AVX is more complex to test, requires 3 checks including
  // _xgetbv(_XCR_XFEATURE_ENABLED_MASK) & 0x6
  // https://insufficientlycomplicated.wordpress.com/2011/11/07/detecting-intel-advanced-vector-extensions-avx-in-visual-studio/
  // https://software.intel.com/en-us/blogs/2011/04/14/is-avx-enabled/
  g_cpuIdInitialized = -1;
}

int hasSse2() {
  if (!g_cpuIdInitialized) {
    initCpuId();
  }
  return g_hasSSE2;
}

void FAR_FN(upsample2f32)(const char* FAR_RESTRICT buf_in,
                          size_t size_in,
                          int sample_rate,
                          int num_channels,
                          char* FAR_RESTRICT buf_out,
                          size_t size_out) {
  if (size_out < size_in * 2)
    return;
  if (/*hasSSE41*/ hasSse2())  // currently uses only SSE, not SSE4.1
  {
    FAR_FN(upsample2f32_sse41)
    (buf_in, size_in, sample_rate, num_channels, buf_out, size_out);
  } else
  {
    FAR_FN(upsample2f32_fpu)
    (buf_in, size_in, sample_rate, num_channels, buf_out, size_out);
  }
}

void FAR_FN(upsample3f32)(const char* FAR_RESTRICT buf_in,
                          size_t size_in,
                          int sample_rate,
                          int num_channels,
                          char* FAR_RESTRICT buf_out,
                          size_t size_out) {
  if (size_out < size_in * 3)
    return;
  if (/*hasSSE41*/ hasSse2())  // currently uses only SSE, not SSE4.1
  {
    FAR_FN(upsample3f32_sse41)
    (buf_in, size_in, sample_rate, num_channels, buf_out, size_out);
  } else {
    FAR_FN(upsample3f32_fpu)
    (buf_in, size_in, sample_rate, num_channels, buf_out, size_out);
  }
}

void FAR_FN(downsample2f32)(const char* FAR_RESTRICT buf_in,
                            size_t size_in,
                            int sample_rate,
                            int num_channels,
                            char* FAR_RESTRICT buf_out,
                            size_t size_out) {
  if (size_out < size_in / 2)
    return;
  if (/*hasSSE41*/ hasSse2())  // currently uses only SSE, not SSE4.1
  {
    FAR_FN(downsample2f32_sse41)
    (buf_in, size_in, sample_rate, num_channels, buf_out, size_out);
  } else {
    FAR_FN(downsample2f32_fpu)
    (buf_in, size_in, sample_rate, num_channels, buf_out, size_out);
  }
}

void FAR_FN(downsample3f32)(const char* FAR_RESTRICT buf_in,
                            size_t size_in,
                            int sample_rate,
                            int num_channels,
                            char* FAR_RESTRICT buf_out,
                            size_t size_out) {
  if (size_out < size_in / 3)
    return;
  if (/*hasSSE41*/ hasSse2())  // currently uses only SSE, not SSE4.1
  {
    FAR_FN(downsample3f32_sse41)
    (buf_in, size_in, sample_rate, num_channels, buf_out, size_out);
  } else {
    FAR_FN(downsample3f32_fpu)
    (buf_in, size_in, sample_rate, num_channels, buf_out, size_out);
  }
}