#pragma once

#include "libfar/base.h"

// Query CPU support for SIMD extensions at runtime.
#include "libfar/far_cpuid.h"

// Adapt channel count (convert mono/stereo).
#include "libfar/adapt.h"

// Resample audio signal (change sampling frequency).
#include "libfar/resample.h"
