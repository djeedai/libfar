
#include "libfar/sse41/adapt_sse41.h"

#include <smmintrin.h>  // SSE4.1

#ifdef __cplusplus
namespace libfar {
#endif  // __cplusplus

void FAR_FN(adapt_ch1x2f32_sse41)(const float128* FAR_RESTRICT buf_in,
                                  size_t size_in,
                                  int sample_rate,
                                  float128* FAR_RESTRICT buf_out,
                                  size_t size_out) {
  size_t num_samples = size_in / sizeof(float);
  const float* src = (const float* FAR_RESTRICT)buf_in;
  float* dst = (float* FAR_RESTRICT)buf_out;
  // 128 blocks x4
  size_t num_blocks = (num_samples / 16);
  for (size_t i = 0; i < num_blocks; ++i) {
    _mm_prefetch((const char*)(src + 4), _MM_HINT_NTA);
    __m128 x0 = _mm_load_ps(src);
    __m128 x1 = _mm_load_ps(src + 1);
    __m128 x2 = _mm_load_ps(src + 2);
    __m128 x3 = _mm_load_ps(src + 3);
    __m128 x0lo = _mm_shuffle_ps(x0, x0, _MM_SHUFFLE(1, 1, 0, 0));
    __m128 x0hi = _mm_shuffle_ps(x0, x0, _MM_SHUFFLE(3, 3, 2, 2));
    __m128 x1lo = _mm_shuffle_ps(x1, x1, _MM_SHUFFLE(1, 1, 0, 0));
    __m128 x1hi = _mm_shuffle_ps(x1, x1, _MM_SHUFFLE(3, 3, 2, 2));
    __m128 x2lo = _mm_shuffle_ps(x2, x2, _MM_SHUFFLE(1, 1, 0, 0));
    __m128 x2hi = _mm_shuffle_ps(x2, x2, _MM_SHUFFLE(3, 3, 2, 2));
    __m128 x3lo = _mm_shuffle_ps(x3, x3, _MM_SHUFFLE(1, 1, 0, 0));
    __m128 x3hi = _mm_shuffle_ps(x3, x3, _MM_SHUFFLE(3, 3, 2, 2));
    _mm_stream_ps(dst, x0lo);
    _mm_stream_ps(dst + 1, x0hi);
    _mm_stream_ps(dst + 2, x1lo);
    _mm_stream_ps(dst + 3, x1hi);
    _mm_stream_ps(dst + 4, x2lo);
    _mm_stream_ps(dst + 5, x2hi);
    _mm_stream_ps(dst + 6, x3lo);
    _mm_stream_ps(dst + 7, x3hi);
    src += 4;
    dst += 8;
  }
  num_samples -= (num_blocks * 16);
  // Remain
  for (size_t i = 0; i < num_samples; ++i) {
    float value = *src++;
    *dst++ = value;  // x1
    *dst++ = value;  // x2
  }
}

void FAR_FN(adapt_ch2x1f32_sse41)(const float128* FAR_RESTRICT buf_in,
                                  size_t size_in,
                                  int sample_rate,
                                  float128* FAR_RESTRICT buf_out,
                                  size_t size_out) {
  size_t num_samples = size_in / sizeof(float);
  const float* src = (const float* FAR_RESTRICT)buf_in;
  float* dst = (float* FAR_RESTRICT)buf_out;
  // 128 blocks x4
  size_t num_blocks = (num_samples / 16);
  for (size_t i = 0; i < num_blocks; ++i) {
    _mm_prefetch((const char*)(src + 4), _MM_HINT_NTA);
    __m128 x0 = _mm_load_ps(src);
    __m128 x1 = _mm_load_ps(src + 1);
    __m128 x2 = _mm_load_ps(src + 2);
    __m128 x3 = _mm_load_ps(src + 3);
    __m128 x0x1 = _mm_shuffle_ps(x0, x1, _MM_SHUFFLE(2, 0, 2, 0));
    __m128 x2x3 = _mm_shuffle_ps(x2, x3, _MM_SHUFFLE(2, 0, 2, 0));
    _mm_stream_ps(dst, x0x1);
    _mm_stream_ps(dst + 1, x2x3);
    src += 4;
    dst += 2;
  }
  num_samples -= (num_blocks * 16);
  // Remain
  for (size_t i = 0; i < num_samples; i += 2) {
    float value = *src++;  // x1 (use)
    ++src;                 // x2 (discard)
    *dst++ = value;
  }
}

void FAR_FN(adapt_ch1x2s16_sse41)(const short128* FAR_RESTRICT buf_in,
                                  size_t size_in,
                                  int sample_rate,
                                  short128* FAR_RESTRICT buf_out,
                                  size_t size_out) {
  size_t num_samples = size_in / sizeof(int16);
  const int16* src = (const int16* FAR_RESTRICT)buf_in;
  int16* dst = (int16 * FAR_RESTRICT) buf_out;
 // // 128 blocks x4
 // size_t num_blocks = (num_samples / /*16*/4);
 // for (size_t i = 0; i < num_blocks; ++i) {
 //   //_mm_prefetch((const char*)(src + 4), _MM_HINT_NTA);
 //   __m128i x0 = _mm_load_si128((const __m128i*)(src));
 //   //__m128i x1 = _mm_load_si128((const __m128i*)(src + 8));
 //   //__m128i x2 = _mm_load_si128((const __m128i*)(src + 16));
 //   //__m128i x3 = _mm_load_si128((const __m128i*)(src + 24));
	//// Start : x0 = [0,1,2,3,4,5,6,7]
	//// Target: x0a = [0,0,1,1,2,2,3,3], x0b = [4,4,5,5,6,6,7,7]
 //   __m128i x0lo = _mm_shuffle_epi32(x0, _MM_SHUFFLE(1, 0, 1, 0)); // [0,1,2,3,0,1,2,3]
 //   __m128i x0hi = _mm_shuffle_epi32(x0, _MM_SHUFFLE(3, 2, 3, 2)); // [4,5,6,7,4,5,6,7]
 //   __m128i x0lo1 = _mm_shufflelo_epi16(x0lo, _MM_SHUFFLE(1, 1, 0, 0)); // [0,0,1,1,0,1,2,3]
	//__m128i x0lo2 = _mm_shufflehi_epi16(x0lo, _MM_SHUFFLE(3, 3, 2, 2)); // [0,1,2,3,2,2,3,3]
 //   __m128i x0a = _mm_blend_epi16(x0lo1, x0lo2, 0b11110000); // [0,0,1,1,2,2,3,3]
 //   __m128i x0hi1 = _mm_shufflelo_epi16(x0hi, _MM_SHUFFLE(1, 1, 0, 0)); // [4,4,5,5,4,5,6,7]
	//__m128i x0hi2 = _mm_shufflehi_epi16(x0hi, _MM_SHUFFLE(3, 3, 2, 2)); // [4,5,6,7,6,6,7,7]
 //   __m128i x0b = _mm_blend_epi16(x0hi1, x0hi2, 0b11110000); // [4,4,5,5,6,6,7,7]
	////todo.. : shuffle then blend is probably shorter : 01234567 => 00114567+01232233 => 00112233
 //   _mm_stream_si128(dst, x0a);
 //   _mm_stream_si128(dst + 1, x0b);
	////todo...
 //   ++src; //src += 4;
 //   dst += 2;  // dst += 8;
 // }
 // num_samples -= (num_blocks * 16);
  // Remain
  for (size_t i = 0; i < num_samples; ++i) {
    int16 value = *src++;
    *dst++ = value;  // x1
    *dst++ = value;  // x2
  }
}

void FAR_FN(adapt_ch2x1s16_sse41)(const short128* FAR_RESTRICT buf_in,
                                  size_t size_in,
                                  int sample_rate,
                                  short128* FAR_RESTRICT buf_out,
                                  size_t size_out) {
  size_t num_samples = size_in / sizeof(int16);
  const int16* src = (const int16* FAR_RESTRICT)buf_in;
  int16* dst = (int16 * FAR_RESTRICT) buf_out;
  //// 128 blocks x4
  // size_t num_blocks = (num_samples / 16);
  // for (size_t i = 0; i < num_blocks; ++i) {
  //  _mm_prefetch((const char*)(src + 4), _MM_HINT_NTA);
  //  __m128 x0 = _mm_load_ps(src);
  //  __m128 x1 = _mm_load_ps(src + 1);
  //  __m128 x2 = _mm_load_ps(src + 2);
  //  __m128 x3 = _mm_load_ps(src + 3);
  //  __m128 x0x1 = _mm_shuffle_ps(x0, x1, _MM_SHUFFLE(2, 0, 2, 0));
  //  __m128 x2x3 = _mm_shuffle_ps(x2, x3, _MM_SHUFFLE(2, 0, 2, 0));
  //  _mm_stream_ps(dst, x0x1);
  //  _mm_stream_ps(dst + 1, x2x3);
  //  src += 4;
  //  dst += 2;
  //}
  // num_samples -= (num_blocks * 16);
  // Remain
  for (size_t i = 0; i < num_samples; i += 2) {
    int16 value1 = *src++;  // x1
    int16 value2 = *src++;  // x2
    *dst++ = (value1 / 2) + (value2 / 2);
  }
}

#ifdef __cplusplus
}  // namespace libfar
#endif  // __cplusplus
