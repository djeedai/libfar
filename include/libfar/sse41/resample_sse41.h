#pragma once

#include "libfar/base.h"

#ifdef __cplusplus
namespace libfar {
#endif  // __cplusplus

void FAR_FN(upsample2f32_sse41)(const byte16* FAR_RESTRICT buf_in,
                                       size_t size_in,
                                       int sample_rate,
                                       int num_channels,
                                       byte16* FAR_RESTRICT buf_out,
                                       size_t size_out);
void FAR_FN(upsample3f32_sse41)(const byte16* FAR_RESTRICT buf_in,
                                       size_t size_in,
                                       int sample_rate,
                                       int num_channels,
                                       byte16* FAR_RESTRICT buf_out,
                                       size_t size_out);
void FAR_FN(downsample2f32_sse41)(const byte16* FAR_RESTRICT buf_in,
                                         size_t size_in,
                                         int sample_rate,
                                         int num_channels,
                                         byte16* FAR_RESTRICT buf_out,
                                         size_t size_out);
void FAR_FN(downsample3f32_sse41)(const byte16* FAR_RESTRICT buf_in,
                                         size_t size_in,
                                         int sample_rate,
                                         int num_channels,
                                         byte16* FAR_RESTRICT buf_out,
                                         size_t size_out);

#ifdef __cplusplus
}  // namespace libfar
#endif  // __cplusplus
