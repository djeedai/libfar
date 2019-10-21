#pragma once

#include "libfar/base.h"

#ifdef __cplusplus
namespace libfar {
extern "C" {
#endif  // __cplusplus

/// Query the CPU for hardware support.
void FAR_API FAR_FN(cpuid_init)();

/// Check for SSE support.
int FAR_API FAR_FN(has_sse)();

/// Check for SSE2 support.
int FAR_API FAR_FN(has_sse2)();

/// Check for SSE4.1 support.
int FAR_API FAR_FN(has_sse41)();

/// Check for SSE4.2 support.
int FAR_API FAR_FN(has_sse42)();

#ifdef __cplusplus
}  // extern "C"
}  // namespace libfar
#endif  // __cplusplus
