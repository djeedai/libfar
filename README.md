# libfar

Fast audio resampling library for C and C++.

This library focuses on speed at the expense of audio quality, for scenarios where correct audio processing is not needed. For that it takes many shortcuts to reduce processing and latency.

If you are looking for a high quality resampling library for offline audio processing, or where audio quality is more important than performance, this project is not suited. Look at a resampler like [the Opus Silk resampler](https://github.com/xiph/opus/blob/master/silk/resampler.c) instead.

## Usage

```c
#include "libfar.h"

// Upsample from 22050 Hz stereo to 44100 Hz stereo.
float* upsample(const float* buffer, size_t size)
{
  size_t size_out = size * 2;
  size_t aligned_size_out = (size_out + 255) & ~(size_t)0xFF;
  char* buf_out = (char*)far_malloc_align(aligned_size_out, 256);
  far_upsample2f32((char*)buffer, size, 22050, 2, buf_out, size_out);
  return (float*)buf_out;
}
```
