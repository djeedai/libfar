#include "pch.h"

TEST(Adapt, Ch1x2f32) {
  float* buf_in = (float*)libfar::malloc_align(76, 64);  // 16N+3
  ASSERT_NE(nullptr, buf_in);
  float* buf_out = (float*)libfar::malloc_align(152, 64);
  ASSERT_NE(nullptr, buf_out);

  for (int i = 0; i < 19; ++i) {
    buf_in[i] = 1.0f / (i + 1.0f);
  }

  libfar::adapt_ch1x2f32((const float512*)buf_in, 76, 48000, (float512*)buf_out,
                         152);

  double err = 0.0;
  float* p = buf_out;
  for (int i = 0; i < 19; ++i) {
    const double l = *p++;
    const double r = *p++;
    const double ref = buf_in[i];
    err += (l - ref) + (r - ref);
  }
  ASSERT_LT(err, 1.0e-10);

  libfar::free_align(buf_in);
  libfar::free_align(buf_out);
}

TEST(Adapt, Ch2x1f32) {
  float* buf_in = (float*)libfar::malloc_align(152, 64);
  ASSERT_NE(nullptr, buf_in);
  float* buf_out = (float*)libfar::malloc_align(76, 64);  // 16N+3
  ASSERT_NE(nullptr, buf_out);

  float* p = buf_in;
  for (int i = 0; i < 19; ++i) {
    *p++ = 1.0f / (i + 1.0f);
    *p++ = 1.0f / (i + 5.0f);
  }

  libfar::adapt_ch2x1f32((const float512*)buf_in, 152, 48000,
                         (float512*)buf_out, 76);

  double err = 0.0;
  p = buf_in;
  for (int i = 0; i < 19; ++i) {
    const double ref_l = *p++;
    const double ref_r = *p++;
    const double ref = (ref_l + ref_r) / 2.0;
    const double out = buf_out[i];
    err += (out - ref);
  }
  ASSERT_LT(err, 1.0e-7);

  libfar::free_align(buf_in);
  libfar::free_align(buf_out);
}

TEST(Adapt, Ch1x2s16) {
  int16* buf_in = (int16*)libfar::malloc_align(70, 64);  // 16N+3
  ASSERT_NE(nullptr, buf_in);
  int16* buf_out = (int16*)libfar::malloc_align(140, 64);
  ASSERT_NE(nullptr, buf_out);

  for (int i = 0; i < 35; ++i) {
    buf_in[i] = (i % 2 == 0 ? -1 : 1) * (i << 9);  // |abs| <= 17920
  }

  libfar::adapt_ch1x2s16((const short512*)buf_in, 70, 48000, (short512*)buf_out,
                         140);

  double err = 0.0;
  int16* p = buf_out;
  for (int i = 0; i < 35; ++i) {
    const double l = *p++;
    const double r = *p++;
    const double ref = buf_in[i];
    err += (l - ref) + (r - ref);
  }
  ASSERT_LT(err, 1.0e-10);

  libfar::free_align(buf_in);
  libfar::free_align(buf_out);
}

TEST(Adapt, Ch2x1s16) {
  int16* buf_in = (int16*)libfar::malloc_align(140, 64);
  ASSERT_NE(nullptr, buf_in);
  int16* buf_out = (int16*)libfar::malloc_align(70, 64);  // 16N+3
  ASSERT_NE(nullptr, buf_out);

  // Keep all samples of same sign to maximize rounding error (worst case
  // scenario) : (r + l) / 2 rounds with 0.5 error per sample.
  int16* p = buf_in;
  for (int i = 0; i < 35; ++i) {
    *p++ = (i << 9) + 5;  // |abs| <= 17925
    *p++ = (i << 9);      // |abs| <= 17920
  }

  libfar::adapt_ch2x1s16((const short512*)buf_in, 140, 48000,
                         (short512*)buf_out, 70);

  double err = 0.0;
  p = buf_in;
  for (int i = 0; i < 35; ++i) {
    const double val = buf_out[i];
    const double ref_l = *p++;
    const double ref_r = *p++;
    const double ref = (ref_l + ref_r) / 2.0;
    err += (val - ref);
  }
  ASSERT_LT(err,
            1.0e-10 + 0.5 * 35);  // can accumulate up to 0.5 per sample due to
                                  // integer rounding in division (r+l)/2

  libfar::free_align(buf_in);
  libfar::free_align(buf_out);
}
