#pragma once

#include "libfar/base.h"

#ifdef __cplusplus
namespace libfar {
#endif  // __cplusplus

/// Convert mono (1 channel) to stereo (2 channels) for float32 values in
/// [-1:1]. The input mono channel is duplicated to both the left and right
/// channels of the stereo output.
void FAR_API FAR_FN(adapt_ch1x2f32_fpu)(const float* FAR_RESTRICT buf_in,
                                        size_t size_in,
                                        int sample_rate,
                                        float* FAR_RESTRICT buf_out,
                                        size_t size_out);

/// Convert stereo (2 channels) to mono (1 channel) for float32 values in
/// [-1:1]. The output mono channel is the average of the left and right
/// channels of the stereo input.
void FAR_API FAR_FN(adapt_ch2x1f32_fpu)(const float* FAR_RESTRICT buf_in,
                                        size_t size_in,
                                        int sample_rate,
                                        float* FAR_RESTRICT buf_out,
                                        size_t size_out);

/// Convert mono (1 channel) to stereo (2 channels) for int16 values. The input
/// mono channel is duplicated to both the left and right channels of the stereo
/// output.
void FAR_API FAR_FN(adapt_ch1x2s16_fpu)(const int16* FAR_RESTRICT buf_in,
                                        size_t size_in,
                                        int sample_rate,
                                        int16* FAR_RESTRICT buf_out,
                                        size_t size_out);

/// Convert stereo (2 channels) to mono (1 channel) for int16 values. The output
/// mono channel is the average of the left and right channels of the stereo
/// input.
void FAR_API FAR_FN(adapt_ch2x1s16_fpu)(const int16* FAR_RESTRICT buf_in,
                                        size_t size_in,
                                        int sample_rate,
                                        int16* FAR_RESTRICT buf_out,
                                        size_t size_out);

#ifdef __cplusplus
}  // namespace libfar
#endif  // __cplusplus
