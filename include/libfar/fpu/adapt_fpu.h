#pragma once

#include "libfar/base.h"

#ifdef __cplusplus
namespace libfar {
extern "C" {
#endif  // __cplusplus

/// Convert mono (1 channel) to stereo (2 channels) for float32 values in
/// [-1:1].
void FAR_API FAR_FN(adapt_ch1x2f32_fpu)(const float* FAR_RESTRICT buf_in,
                                        size_t size_in,
                                        int sample_rate,
                                        float* FAR_RESTRICT buf_out,
                                        size_t size_out);

/// Convert stereo (2 channels) to mono (1 channel) for float32 values in
/// [-1:1].
void FAR_API FAR_FN(adapt_ch2x1f32_fpu)(const float* FAR_RESTRICT buf_in,
                                        size_t size_in,
                                        int sample_rate,
                                        float* FAR_RESTRICT buf_out,
                                        size_t size_out);

/// Convert mono (1 channel) to stereo (2 channels) for int16 values.
void FAR_API FAR_FN(adapt_ch1x2s16_fpu)(const int16* FAR_RESTRICT buf_in,
                                        size_t size_in,
                                        int sample_rate,
                                        int16* FAR_RESTRICT buf_out,
                                        size_t size_out);

/// Convert stereo (2 channels) to mono (1 channel) for int16 values.
void FAR_API FAR_FN(adapt_ch2x1s16_fpu)(const int16* FAR_RESTRICT buf_in,
                                        size_t size_in,
                                        int sample_rate,
                                        int16* FAR_RESTRICT buf_out,
                                        size_t size_out);

#ifdef __cplusplus
}  // extern "C"
}  // namespace libfar
#endif  // __cplusplus
