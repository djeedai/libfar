#pragma once

// Standard headers
#if !defined(FAR_NO_STD_INCLUDES)
#ifdef __cplusplus
#include <cstdlib>
#else  // __cplusplus
#include <stdlib.h>
#ifndef _MSC_VER
#include <stdalign.h>
#endif  // _MSC_VER
#endif  // __cplusplus
#endif  // !defined(FAR_NO_STD_INCLUDES)

#define FAR_CONCAT(a, b) a##b

// Default function naming strategy
// - In C++, use a namespace
// - In C, use a prefix
#if !defined(FAR_FN_PREFIX)
#ifdef __cplusplus
// No prefix, namespace instead
#define FAR_FN_PREFIX
#else  // __cplusplus
// Prefix to avoid global namespace collision
#define FAR_FN_PREFIX far_
#endif  // __cplusplus
#endif  // !defined(FAR_FN_PREFIX)
#define FAR_FN(x) FAR_CONCAT(far_, x)

// To allow shared library compiling
#if !defined(FAR_API)
#define FAR_API
#endif

// Aligned allocations for SIMD
#if !defined(FAR_CUSTOM_ALLOCATORS)
#ifdef _MSC_VER
#ifdef __cplusplus
#define malloc_align(s, a) _aligned_malloc(s, a)
#define free_align(p) _aligned_free(p)
#else  // __cplusplus
#define far_malloc_align(s, a) _aligned_malloc(s, a)
#define far_free_align(p) _aligned_free(p)
#endif  // __cplusplus
#else   // _MSC_VER
#if (defined(DEBUG) || defined(_DEBUG)) && !defined(NDEBUG)
inline void* FAR_FN(malloc_align)(size_t size, size_t align) {
  assert(size % align == 0);
  return aligned_alloc(align, size);
};
#else  // debug
#ifdef __cplusplus
#define malloc_align(s, a) aligned_alloc(a, s)
#else  // __cplusplus
#define far_malloc_align(s, a) aligned_alloc(a, s)
#endif  // __cplusplus
#endif  // debug
#ifdef __cplusplus
#define free_align(p) free(p)
#else  // __cplusplus
#define far_free_align(p) free(p)
#endif  // __cplusplus
#endif
#endif  //! defined(FAR_CUSTOM_ALLOCATORS)

// Pointer aliasing optimization
#define FAR_RESTRICT __restrict

#ifdef _MSC_VER
#include <intrin.h>
#endif
