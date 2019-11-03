# libfar

Fast audio resampling library for C and C++.

This library focuses on speed at the expense of audio quality, for scenarios where correct audio processing is not needed. For that it takes many shortcuts to reduce processing and latency.

If you are looking for a high quality resampling library for offline audio processing, or where audio quality is more important than performance, this project is not suited. Look at a resampler like [the Opus Silk resampler](https://github.com/xiph/opus/blob/master/silk/resampler.c) instead.

## Usage

**C**

```c
#include "libfar.h"

// Upsample float stereo audio from 22050 Hz to 44100 Hz.
byte32* upsample(const byte32* buffer, size_t size)
{
  const size_t size_out = size * 2;
  const size_t aligned_size_out = (size_out + 31) & ~(size_t)31;
  char32* buf_out = far_malloc_align32(aligned_size_out);
  const int num_channels = 2; // stereo
  far_upsample2f32(buffer, size, 22050, num_channels, buf_out, size_out);
  return buf_out;
}
```

**C++**

```cpp
#include "libfar.h"

// Convert int16 48kHz audio from mono to stereo.
short512* mono_to_stereo(const short512* buffer, size_t size)
{
  const size_t size_out = size * 2;
  const size_t aligned_size_out = (size_out + 63) & ~(size_t)63;
  short512* buf_out = libfar::malloc_align(aligned_size_out, 64);
  libfar::adapt_ch1x2s16(buffer, size, 48000, buf_out, size_out);
  return buf_out;
}
```

## Coding conventions

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

### Audio sample types

`libfar` supports 3 different audio sample types:

| Sample type | Kind | Size | Range | Description |
|---|---|---|---|---|
| `uint8` | Integer | 1 byte | [0:255] | Low-resolution 8-bit format, generally only used to achieve retro 8-bit effects. Avoid this format for anything else, as its resolution is too low and produces distorsions. |
| `int16` | Integer | 2 bytes | [-32768:32767] | Most common audio sample format for playback and storage. |
| `float` | Floating-point | 4 bytes | [-1.0:1.0] | Used by some game engines (Unity3D) or as intermediate type for audio pipelines while applying effects to retain a high precision and avoid distorsion. |

### Boolean type

**C**

In C, there is no built-in boolean type. As a fallback, `libfar` defines a `far_bool` type backed by an `int32_t` type and equal to `0` for `false` and `-1` (bit pattern 0xFFFFFFFF) for `true`.

```c
typedef int32_t far_bool;
#define far_true ((far_bool)-1)
#define far_false ((far_bool)0)
```

**C++**

In C++ for convenience `far_bool` is an alias of the built-in boolean type `bool`. Developers are encouraged to use `bool` directly.

```cpp
using far_bool = bool;
constexpr bool far_true = true;
constexpr bool far_false = false;
```

### Buffer alignment

Buffers are over-aligned compared to standard C/C++ rules to improve performance with SIMD. Special types aliases are defined for those buffers to make the alignment requirement explicit via the type name. There is currently no compile-time check, and it is still possible to pass the wrong type, so developers should be careful.

| Sample type | Alignment | Aligned type alias |
|---|---|---|---|
| Unspecified* | 4 bytes | `byte4` |
| Unspecified* | 8 bytes | `byte8` |
| Unspecified* | 16 bytes | `byte16` |
| Unspecified* | 32 bytes | `byte32` |
| `int16` | 2 bytes | `int16` |
| `int16` | 16 bytes | `short128` |
| `int16` | 32 bytes | `short256` |
| `int16` | 32 bytes | `short512` |
| `float` | 4 bytes | `float` |
| `float` | 16 bytes | `float128` |
| `float` | 32 bytes | `float256` |
| `float` | 32 bytes | `float512` |

**Unspecified sample types indicate only an alignment, without specifying an actual type. This is useful for functions which can operate on any sample type, but still require an aligned buffer.*

Typically the following SIMD implementations requires some specific alignment:

- **SSE, SSE2, ..., SSE4.2** : 16 bytes (128 bits)
- **AVX, AVX2** : 32 bytes  (256 bits)
- **AVX512** : 64 bytes (512 bits)

Types of a given alignment can safely be cast to a lower alignment. Therefore most generic dispatching functions take some 64-byte aligned buffers to not lose any generality.

**C++**

```cpp
// Generic dispatcher function func1().
byte16* ptr16 = ...
libfar::func1((const byte4*)ptr16, ...); // OK; cast to lower alignment.

// Specific SSE implementation of func1()
byte16* ptr16 = ...
libfar::func1_sse(ptr16, ...); // OK; SSE explicitly requires 16-byte alignment.
```

### Memory allocators

Default allocators are based on the standard library's `aligned_alloc()`. Custom allocators can be defined by defining the `FAR_CUSTOM_ALLOCATORS` preprocessor symbol to avoid defining the built-in allocators, and defining them manually instead.

_Note_: If defining the allocators generically for both C and C++, remember to use the `FAR_FN()` macro. Otherwise you can use directly the prefix (default: `far_`) for C or the namespace (`libfar`) for C++.

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

**C++**

On C++ only, some templated versions are provided for convenience, which deduct the alignment from the sample type alias.

```cpp
short512* buffer = libfar::malloc_align<short512>(size); // 64-byte aligned
float128* buffer = libfar::malloc_align<float128>(size); // 16-byte aligned
```

## Feature reference

### Resampling

Change the sample rate by an integral factor.

| Name | Sample type | Description |
|---|---|---|
| `upsample2f32` | `float` | Upsample 1:2 |
| `upsample3f32` | `float` | Upsample 1:3 |
| `downsample2f32` | `float` | Downsample 2:1 |
| `downsample3f32` | `float` | Downsample 3:1 |

**C**

```c
size_t size_out = 3 * size_in;
float512* buf_out =(float512*)far_malloc_align64(size_out);
far_upsample3f32(buf_in, size_in, 48000, 2, buf_out, size_out);
```

**C++**

```cpp
size_t size_out = size_in / 2;
float512* buf_out = libfar::malloc_align<float512>(size_out);
libfar::downsample2f32(buf_in, size_in, 44100, 1, buf_out, size_out);
```

### Channel adapting

Change the number of audio channels.

| Name | Sample type | Input | Output |
|---|---|---|---|
| `adapt_ch1x2s16` | `int16` | Mono (1) | Stereo (2) |
| `adapt_ch1x2f32` | `float` | Mono (1) | Stereo (2) |
| `adapt_ch2x1s16` | `int16` | Stereo (2) | Mono (1) |
| `adapt_ch2x1f32` | `float` | Stereo (2) | Mono (1) |

### Memory

Utilities for allocating and deallocating aligned memory buffers.

| Name | Description |
|---|---|
| `malloc_align(size, align)` | Allocate a block of memory with the given size and alignment. |
| `free_align(ptr)` | Free a block of memory allocated with `malloc_align`. |
| `malloc_align16(size)` | Allocate a 16-byte aligned block of memory with the given size. Default implementation is a shorthand for `malloc_align(size, 16)`. Custom implementations can optimize the allocation based on the compile-time size. |
| `free_align16(ptr)` | Free a block of memory allocated with `malloc_align16`. |
| `malloc_align32(size)` | Allocate a 32-byte aligned block of memory with the given size. Default implementation is a shorthand for `malloc_align(size, 32)`. Custom implementations can optimize the allocation based on the compile-time size. |
| `free_align32(ptr)` | Free a block of memory allocated with `malloc_align32`. |
| `malloc_align64(size)` | Allocate a 64-byte aligned block of memory with the given size. Default implementation is a shorthand for `malloc_align(size, 64)`. Custom implementations can optimize the allocation based on the compile-time size. |
| `free_align64(ptr)` | Free a block of memory allocated with `malloc_align64`. |

**C++**

In C++, an extra templated helper is available which deduces the alignment from the type alias, which redirects to the corresponding `malloc_alignN()` variant, where `N` is the deduced alignment.

```cpp
template <typename T>
T* malloc_align(size_t size);
```

The template is only instantiated for the `intX` and `floatX` type aliases.

| Template instantiation | Returned buffer alignment | Implementation |
|---|---|---|
| `malloc_align<float128>(size)` | 16 bytes | `malloc_align16()` |
| `malloc_align<float256>(size)` | 32 bytes | `malloc_align32()` |
| `malloc_align<float512>(size)` | 64 bytes | `malloc_align64()` |
| `malloc_align<short128>(size)` | 16 bytes | `malloc_align16()` |
| `malloc_align<short256>(size)` | 32 bytes | `malloc_align32()` |
| `malloc_align<short512>(size)` | 64 bytes | `malloc_align64()` |

### CPUID

Utilities to detect CPU features at runtime and dynamically select the fastest implementation of each library function available on the current host CPU. This is generally used internally only.

| Name | Description |
|---|---|
| `void cpuid_init()` | Query and cache the hardware feature support for the host machine. Called once automatically before any of the other functions below; the result is cached after that. |
| `far_bool has_sse()` | Return `far_true` if SSE is available on the host machine. |
| `far_bool has_sse2()` | Return `far_true` if SSE2 is available on the host machine. |
| `far_bool has_sse3()` | Return `far_true` if SSE3 is available on the host machine. |
| `far_bool has_ssse3()` | Return `far_true` if SSSE3 is available on the host machine. |
| `far_bool has_sse41()` | Return `far_true` if SSE4.1 is available on the host machine. |
| `far_bool has_sse42()` | Return `far_true` if SSE4.2 is available on the host machine. |
