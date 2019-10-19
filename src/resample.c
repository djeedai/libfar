
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

void FAR_FN(upsample2f32)(const char* FAR_RESTRICT buf_in,
                          size_t size_in,
                          int sample_rate,
                          int num_channels,
                          char* FAR_RESTRICT buf_out,
                          size_t size_out) {
  if (size_out < size_in * 2)
    return;
  //< TODO - use CPUID
  FAR_FN(upsample2f32_sse41)
  (buf_in, size_in, sample_rate, num_channels, buf_out, size_out);
}

void FAR_FN(upsample3f32)(const char* FAR_RESTRICT buf_in,
                          size_t size_in,
                          int sample_rate,
                          int num_channels,
                          char* FAR_RESTRICT buf_out,
                          size_t size_out) {
  if (size_out < size_in * 3)
    return;
  //< TODO - use CPUID
  FAR_FN(upsample3f32_sse41)
  (buf_in, size_in, sample_rate, num_channels, buf_out, size_out);
}

void FAR_FN(downsample2f32)(const char* FAR_RESTRICT buf_in,
                            size_t size_in,
                            int sample_rate,
                            int num_channels,
                            char* FAR_RESTRICT buf_out,
                            size_t size_out) {
  if (size_out < size_in / 2)
    return;
  //< TODO - use CPUID
  FAR_FN(downsample2f32_sse41)
  (buf_in, size_in, sample_rate, num_channels, buf_out, size_out);
}

void FAR_FN(downsample3f32)(const char* FAR_RESTRICT buf_in,
                            size_t size_in,
                            int sample_rate,
                            int num_channels,
                            char* FAR_RESTRICT buf_out,
                            size_t size_out) {
  if (size_out < size_in / 3)
    return;
  //< TODO - use CPUID
  FAR_FN(downsample3f32_sse41)
  (buf_in, size_in, sample_rate, num_channels, buf_out, size_out);
}