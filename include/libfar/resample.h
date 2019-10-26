#pragma once

#include "libfar/base.h"

#ifdef __cplusplus
namespace libfar {
#endif  // __cplusplus

/// Upsample from given rate to 2x the given rate (e.g. 24kHz -> 48kHz)
/// for float32 values in [-1:1].
void FAR_API FAR_FN(upsample2f32)(const byte32* FAR_RESTRICT buf_in,
                                  size_t size_in,
                                  int sample_rate,
                                  int num_channels,
                                  byte32* FAR_RESTRICT buf_out,
                                  size_t size_out);

/// Upsample from given rate to 3x the given rate (e.g. 16kHz -> 48kHz)
/// for float32 values in [-1:1].
void FAR_API FAR_FN(upsample3f32)(const byte32* FAR_RESTRICT buf_in,
                                  size_t size_in,
                                  int sample_rate,
                                  int num_channels,
                                  byte32* FAR_RESTRICT buf_out,
                                  size_t size_out);

/// Downsample from given rate to /2 the given rate (e.g. 48kHz -> 24kHz)
/// for float32 values in [-1:1].
void FAR_API FAR_FN(downsample2f32)(const byte32* FAR_RESTRICT buf_in,
                                    size_t size_in,
                                    int sample_rate,
                                    int num_channels,
                                    byte32* FAR_RESTRICT buf_out,
                                    size_t size_out);

/// Downsample from given rate to /3 the given rate (e.g. 48kHz -> 16kHz)
/// for float32 values in [-1:1].
void FAR_API FAR_FN(downsample3f32)(const byte32* FAR_RESTRICT buf_in,
                                    size_t size_in,
                                    int sample_rate,
                                    int num_channels,
                                    byte32* FAR_RESTRICT buf_out,
                                    size_t size_out);

#ifdef __cplusplus
}  // namespace libfar
#endif  // __cplusplus
