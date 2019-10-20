# libfar

Fast audio resampling library for C and C++.

This library focuses on speed at the expense of audio quality, for scenarios where correct audio processing is not needed. For that it takes many shortcuts to reduce processing and latency.

If you are looking for a high quality resampling library for offline audio processing, or where audio quality is more important than performance, this project is not suited. Look at a resampler like [the Opus Silk resampler](https://github.com/xiph/opus/blob/master/silk/resampler.c) instead.

## Usage

```c
#include "libfar.h"

// Upsample from 22050 Hz stereo to 44100 Hz stereo.
byte32* upsample(const byte32* buffer, size_t size)
{
  const size_t size_out = size * 2;
  const size_t aligned_size_out = (size_out + 31) & ~(size_t)0x1F;
  char32* buf_out = far_malloc_align32(aligned_size_out);
  far_upsample2f32((char*)buffer, size, 22050, 2, buf_out, size_out);
  return buf_out;
}
```

## Conventions

### Symbols scoping

Internally functions names are wrapped into the `FAR_FN` macro to allow inserting an optional prefix for C usage (defined by `FAR_FN_PREFIX`).

**C**

Functions are prefixed with `FAR_FN_PREFIX`, which defaults to `far_` but can be redefined by the user:

```cpp
far_upsample2f32(...);
```

**C++**

Functions are enclosed inside the `libfar` namespace:

```cpp
libfar::upsample2f32(...);
```

The `FAR_FN_PREFIX` preprocessor definition is ignored.

### Buffer alignment

Buffers are over-aligned compared to standard C/C++ rules to improve performance with SIMD. Special types are defined for those buffers to avoid accidental mixing:

- 4-byte aligned : `byte4`
- 8-byte aligned : `byte8`
- 16-byte aligned : `byte16`
- 32-byte aligned : `byte32`

**C++**

```cpp
byte16* ptr16 = ...
libfar::func((const byte4*)ptr16, ...); // OK

byte4* ptr4 = ...
libfar::func((const byte16*)ptr4, ...); // Invalid
```

### Memory allocators

Default allocators are based on the standard library's `aligned_alloc()`. Custom allocators can be defined by defining the `FAR_CUSTOM_ALLOCATORS` preprocessor symbol to avoid defining the built-in allocators, and defining them manually instead. If defining the allocators generically for C/C++, remember to use the `FAR_FN()` macro.

```c
char* FAR_FN(malloc_align)(size_t size, size_t align) {
  // ...
}

void FAR_FN(free_align)(void* ptr) {
  // ...
}

byte32* FAR_FN(malloc_align32)(size_t size) {
  // ...
}

void FAR_FN(free_align32)(byte32* ptr) {
  // ...
}
```
