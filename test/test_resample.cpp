#include "pch.h"

TEST(Resample, Upsample2f32) {
  float* buf_in = (float*)libfar::malloc_align(64, 64);
  ASSERT_NE(nullptr, buf_in);
  float* buf_out = (float*)libfar::malloc_align(128, 64);
  ASSERT_NE(nullptr, buf_out);

  for (int i = 0; i < 16; ++i) {
    buf_in[i] = 1.0f / (i + 1.0f);
  }

  libfar::upsample2f32((const byte32*)buf_in, 64, 48000, 1, (byte32*)buf_out,
                       128);

  //< TODO - Actual testing requires some FFT to check that the sound produced
  // is "similar enough" to the input one, which is not possible to determine
  // from the time series.

  // double err = 0.0;
  // float* p = buf_out;
  // for (int i = 0; i < 32; ++i) {
  //  const double value = *p++;
  //  const double r = *p++;
  //  const double ref = buf_in[i];
  //  err += (l - ref) + (r - ref);
  //}
  // ASSERT_LT(err, 1.0e-10);

  libfar::free_align(buf_in);
  libfar::free_align(buf_out);
}