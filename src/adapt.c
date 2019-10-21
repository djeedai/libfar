
#include "libfar/base.h"
#include "libfar/cpuid.h"

#include "libfar/fpu/adapt_fpu.h"
#include "libfar/sse41/adapt_sse41.h"

#ifdef __cplusplus
namespace libfar {
#endif  // __cplusplus

void FAR_API FAR_FN(adapt_ch1x2f32)(const float512* FAR_RESTRICT buf_in,
                                    size_t size_in,
                                    int sample_rate,
                                    float512* FAR_RESTRICT buf_out,
                                    size_t size_out) {
  if (size_out < size_in * 2)
    return;
  if (FAR_FN(has_sse)())  // currently uses only SSE, not SSE4.1
  {
    FAR_FN(adapt_ch1x2f32_sse41)
    ((const float128*)buf_in, size_in, sample_rate, (float128*)buf_out,
     size_out);
  } else {
    FAR_FN(adapt_ch1x2f32_fpu)
    ((const float*)buf_in, size_in, sample_rate, (float*)buf_out, size_out);
  }
}

void FAR_API FAR_FN(adapt_ch2x1f32)(const float512* FAR_RESTRICT buf_in,
                                    size_t size_in,
                                    int sample_rate,
                                    float512* FAR_RESTRICT buf_out,
                                    size_t size_out) {
  if (size_out < size_in / 2)
    return;
  if (FAR_FN(has_sse)())  // currently uses only SSE, not SSE4.1
  {
    FAR_FN(adapt_ch2x1f32_sse41)
    ((const float128*)buf_in, size_in, sample_rate, (float128*)buf_out,
     size_out);
  } else {
    FAR_FN(adapt_ch2x1f32_fpu)
    ((const float*)buf_in, size_in, sample_rate, (float*)buf_out, size_out);
  }
}

void FAR_API FAR_FN(adapt_ch1x2s16)(const short512* FAR_RESTRICT buf_in,
                                    size_t size_in,
                                    int sample_rate,
                                    short512* FAR_RESTRICT buf_out,
                                    size_t size_out) {
  if (size_out < size_in * 2)
    return;
  if (FAR_FN(has_sse)())  // currently uses only SSE, not SSE4.1
  {
    FAR_FN(adapt_ch1x2s16_sse41)
    ((const short128*)buf_in, size_in, sample_rate, (short128*)buf_out,
     size_out);
  } else {
    FAR_FN(adapt_ch1x2s16_fpu)
    ((const int16*)buf_in, size_in, sample_rate, (int16*)buf_out, size_out);
  }
}

void FAR_API FAR_FN(adapt_ch2x1s16)(const short512* FAR_RESTRICT buf_in,
                                    size_t size_in,
                                    int sample_rate,
                                    short512* FAR_RESTRICT buf_out,
                                    size_t size_out) {
  if (size_out < size_in / 2)
    return;
  if (FAR_FN(has_sse)())  // currently uses only SSE, not SSE4.1
  {
    FAR_FN(adapt_ch2x1s16_sse41)
    ((const short128*)buf_in, size_in, sample_rate, (short128*)buf_out,
     size_out);
  } else {
    FAR_FN(adapt_ch2x1s16_fpu)
    ((const int16*)buf_in, size_in, sample_rate, (int16*)buf_out, size_out);
  }
}

#ifdef __cplusplus
}  // namespace libfar
#endif  // __cplusplus