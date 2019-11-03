#pragma once

#include "libfar/libfar_config.h"
#include "libfar/libfar_version.h"

// Standard headers
#if !defined(FAR_NO_STD_INCLUDES)
#ifdef __cplusplus
#include <cstdlib>
#else  // __cplusplus
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __clang__
#include <stdalign.h>
// Should be defined in assert.h in C11, but clang-cl is missing it
#define static_assert(b, m) _Static_assert(b, m)
#endif  // _MSC_VER
#endif  // __cplusplus
#endif  // !defined(FAR_NO_STD_INCLUDES)

#define FAR_XCONCAT(a, b) a##b
#define FAR_CONCAT(a, b) FAR_XCONCAT(a, b)

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
#define FAR_FN(x) FAR_CONCAT(FAR_FN_PREFIX, x)

// To allow shared library compiling
#if !defined(FAR_API)
#ifdef _MSC_VER
#define FAR_API
#else
#define FAR_API __attribute__((visibility("default")))
#endif
#endif

// Pointer aliasing optimization
#ifdef _MSC_VER
#define FAR_RESTRICT __restrict
#elif defined(__cplusplus)
#define FAR_RESTRICT __restrict__
#else  // _MSC_VER
#define FAR_RESTRICT restrict
#endif  // _MSC_VER

// Unused parameter
#define FAR_UNUSED(x) (void)x

// Boolean
#ifdef __cplusplus
using far_bool = bool;
constexpr bool far_true = true;
constexpr bool far_false = false;
#else  // __cplusplus
typedef int32_t far_bool;
#define far_true ((far_bool)-1)
#define far_false ((far_bool)0)
#endif  // __cplusplus

// Base types
typedef unsigned char uint8;
static_assert(sizeof(uint8) == 1, "");
typedef signed short int16;
static_assert(sizeof(int16) == 2, "");

// Aligned types for input and output buffers
#ifdef _MSC_VER
typedef struct byte4 {
  char __declspec(align(4)) c;
} byte4;
typedef struct byte8 {
  char __declspec(align(8)) c;
} byte8;
typedef struct byte16 {
  char __declspec(align(16)) c;
} byte16;
typedef struct byte32 {
  char __declspec(align(32)) c;
} byte32;
typedef struct byte64 {
  char __declspec(align(64)) c;
} byte64;
typedef struct __declspec(align(16)) short128 {
  int16 c[8];
} short128;
static_assert(sizeof(short128) == 16, "");
typedef struct __declspec(align(32)) short256 {
  int16 c[16];
} short256;
static_assert(sizeof(short256) == 32, "");
typedef struct __declspec(align(64)) short512 {
  int16 c[32];
} short512;
static_assert(sizeof(short512) == 64, "");
typedef struct __declspec(align(16)) float128 {
  float c[4];
} float128;
static_assert(sizeof(float128) == 16, "");
typedef struct __declspec(align(32)) float256 {
  float c[8];
} float256;
static_assert(sizeof(float256) == 32, "");
typedef struct __declspec(align(64)) float512 {
  float c[16];
} float512;
static_assert(sizeof(float512) == 64, "");
#else   // _MSC_VER
typedef char __attribute__((align(4))) byte4;
typedef char __attribute__((align(8))) byte4;
typedef char __attribute__((align(16)) byte16;
typedef char __attribute__((align(32)) byte32;
#endif  // _MSC_VER

// Aligned allocations for SIMD
#if !defined(FAR_CUSTOM_ALLOCATORS)
#ifdef __cplusplus
#include "libfar/far_memory.hxx"
#else
#include "libfar/far_memory.h"
#endif
#endif  //! defined(FAR_CUSTOM_ALLOCATORS)

#ifdef _MSC_VER
#include <intrin.h>
#endif  // _MSC_VER
