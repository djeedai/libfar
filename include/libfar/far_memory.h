#pragma once

#include "libfar/base.h"

#if !defined(__cplusplus)

//
// Generic aligned allocator
//

#ifdef _MSC_VER

// MSVC's _aligned_malloc() handles any size

inline void* FAR_FN(malloc_align)(size_t size, size_t align) {
  return _aligned_malloc(size, align);
}

inline void FAR_FN(free_align)(void* ptr) {
  _aligned_free(ptr);
}

#else  // _MSC_VER

// C11's aligned_alloc requires a size multiple of the alignment

inline void* FAR_FN(malloc_align)(size_t size, size_t align) {
  const size_t padded_size = (size + align - 1);
  const size_t aligned_size = padded_size - (padded_size % align);
  // TODO - check this and remove
  assert(aligned_size % align == 0);
  if (size % align == 0) {
    assert(aligned_size == size);
  }
  return aligned_alloc(align, aligned_size);
}

inline void FAR_FN(free_align)(void* ptr) {
  free(ptr);
}

#endif  // _MSC_VER

//
// Compile-time alignment allocators
//

#ifdef _MSC_VER

// MSVC's _aligned_malloc() handles any size

inline byte16* FAR_FN(malloc_align16)(size_t size) {
  return (byte16*)_aligned_malloc(size, 16);
}

inline byte32* FAR_FN(malloc_align32)(size_t size) {
  return (byte32*)_aligned_malloc(size, 32);
}

inline byte64* FAR_FN(malloc_align64)(size_t size) {
  return (byte64*)_aligned_malloc(size, 64);
}

inline void FAR_FN(free_align16)(byte16* ptr) {
  _aligned_free(ptr);
}

inline void FAR_FN(free_align32)(byte32* ptr) {
  _aligned_free(ptr);
}

inline void FAR_FN(free_align64)(byte64* ptr) {
  _aligned_free(ptr);
}

#else  // _MSC_VER

// C11's aligned_alloc requires a size multiple of the alignment

inline byte16* FAR_FN(malloc_align16)(size_t size) {
  return (byte16*)aligned_alloc(16, (size + 15) & ~((size_t)15));
}

inline byte32* FAR_FN(malloc_align32)(size_t size) {
  return (byte32*)aligned_alloc(32, (size + 31) & ~((size_t)31));
}

inline byte64* FAR_FN(malloc_align64)(size_t size) {
  return (byte64*)aligned_alloc(64, (size + 63) & ~((size_t)63));
}

inline void FAR_FN(free_align16)(byte16* ptr) {
  free(ptr);
}

inline void FAR_FN(free_align32)(byte32* ptr) {
  free(ptr);
}

inline void FAR_FN(free_align64)(byte64* ptr) {
  free(ptr);
}

#endif  // _MSC_VER

#endif  // !defined(__cplusplus)
