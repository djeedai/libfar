#pragma once

#include "libfar/base.h"

#ifdef __cplusplus
namespace libfar {
#endif  // __cplusplus

/// Query the CPU for hardware support. The result is cached internally such
/// that subsequent calls are no-op.
void FAR_API FAR_FN(cpuid_init)();

/// Check for SSE support. This will call |cpuid_init| if not already called.
int FAR_API FAR_FN(has_sse)();

/// Check for SSE2 support. This will call |cpuid_init| if not already called.
int FAR_API FAR_FN(has_sse2)();

/// Check for SSE4.1 support. This will call |cpuid_init| if not already called.
int FAR_API FAR_FN(has_sse41)();

/// Check for SSE4.2 support. This will call |cpuid_init| if not already called.
int FAR_API FAR_FN(has_sse42)();

#ifdef __cplusplus
}  // namespace libfar
#endif  // __cplusplus
