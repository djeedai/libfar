#pragma once

#include "libfar/base.h"

#ifdef __cplusplus

#include <cassert>

namespace libfar {

//
// Generic aligned allocator
//

#ifdef _MSC_VER

// MSVC's _aligned_malloc() handles any size

inline void* malloc_align(size_t size, size_t align) {
  return _aligned_malloc(size, align);
}

inline void free_align(void* ptr) {
  _aligned_free(ptr);
}

#else  // _MSC_VER

// C11's aligned_alloc requires a size multiple of the alignment

inline void* malloc_align(size_t size, size_t align) FAR_ALLOC_SIZE(1) FAR_ALIGNED_RET(2) {
  const size_t padded_size = (size + align - 1);
  const size_t aligned_size = padded_size - (padded_size % align);
  // TODO - check this and remove
  assert(aligned_size % align == 0);
  if (size % align == 0) {
    assert(aligned_size == size);
  }
  return aligned_alloc(align, aligned_size);
}

inline void free_align(void* ptr) {
  free(ptr);
}

#endif  // _MSC_VER

//
// Compile-time alignment allocators
//

#ifdef _MSC_VER

// MSVC's _aligned_malloc() handles any size

inline byte16* malloc_align16(size_t size) {
  return (byte16*)malloc_align(size, 16);
}

inline byte32* malloc_align32(size_t size) {
  return (byte32*)malloc_align(size, 32);
}

inline byte64* malloc_align64(size_t size) {
  return (byte64*)malloc_align(size, 64);
}

#else  // _MSC_VER

// C11's aligned_alloc requires a size multiple of the alignment

inline byte16* malloc_align16(size_t size) FAR_ALLOC_SIZE(1) {
  return (byte16*)malloc_align(16, (size + 15) & ~((size_t)15));
}
inline byte32* malloc_align32(size_t size) FAR_ALLOC_SIZE(1) {
  return (byte32*)malloc_align(32, (size + 31) & ~((size_t)31));
}

inline byte64* malloc_align64(size_t size) FAR_ALLOC_SIZE(1) {
  return (byte64*)malloc_align(64, (size + 63) & ~((size_t)63));
}

#endif  // _MSC_VER

inline void free_align16(byte16* ptr) {
  free_align((char*)ptr);
}

inline void free_align32(byte32* ptr) {
  free_align((char*)ptr);
}

inline void free_align64(byte64* ptr) {
  free_align((char*)ptr);
}


//
// Templated helper with alignment deduction
//

template <typename T>
T* malloc_align(size_t size);

template <>
inline float128* malloc_align(size_t size) FAR_ALLOC_SIZE(1) {
  return reinterpret_cast<float128*>(malloc_align16(size));
}

template <>
inline float256* malloc_align(size_t size) FAR_ALLOC_SIZE(1) {
  return reinterpret_cast<float256*>(malloc_align32(size));
}

template <>
inline float512* malloc_align(size_t size) FAR_ALLOC_SIZE(1) {
  return reinterpret_cast<float512*>(malloc_align64(size));
}

template <>
inline short128* malloc_align(size_t size) FAR_ALLOC_SIZE(1) {
  return reinterpret_cast<short128*>(malloc_align16(size));
}

template <>
inline short256* malloc_align(size_t size) FAR_ALLOC_SIZE(1) {
  return reinterpret_cast<short256*>(malloc_align32(size));
}

template <>
inline short512* malloc_align(size_t size) FAR_ALLOC_SIZE(1) {
  return reinterpret_cast<short512*>(malloc_align64(size));
}

}  // namespace libfar
#endif  // __cplusplus
