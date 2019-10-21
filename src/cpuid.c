
#include "libfar/base.h"
#include "libfar/cpuid.h"

static int g_cpuIdInitialized = 0;
static int g_hasSSE = 0;
static int g_hasSSE2 = 0;
static int g_hasSSE41 = 0;
static int g_hasSSE42 = 0;

#ifdef __cplusplus
namespace libfar {
#endif  // __cplusplus

void FAR_FN(cpuid_init)() {
  int cpuinfo[4];
  __cpuid(cpuinfo, 0);
  int numIds = cpuinfo[0];
  int ecx1 = 0, edx1 = 0;
  if (numIds >= 1) {
    __cpuid(cpuinfo, 1);
    ecx1 = cpuinfo[2];
    edx1 = cpuinfo[3];
  }
  int ecx7 = 0, edx7 = 0;
  if (numIds >= 7) {
    __cpuid(cpuinfo, 7);
    ecx7 = cpuinfo[2];
    edx7 = cpuinfo[3];
  }
  g_hasSSE = (edx1 & 0x02000000);
  g_hasSSE2 = (edx1 & 0x04000000);
  g_hasSSE41 = (ecx1 & 0x00080000);
  g_hasSSE42 = (ecx1 & 0x00100000);
  // Support for AVX is more complex to test, requires 3 checks including
  // _xgetbv(_XCR_XFEATURE_ENABLED_MASK) & 0x6
  // https://insufficientlycomplicated.wordpress.com/2011/11/07/detecting-intel-advanced-vector-extensions-avx-in-visual-studio/
  // https://software.intel.com/en-us/blogs/2011/04/14/is-avx-enabled/
  g_cpuIdInitialized = -1;
}

int FAR_FN(has_sse)() {
  if (!g_cpuIdInitialized) {
    FAR_FN(cpuid_init)();
  }
  return g_hasSSE;
}

int FAR_FN(has_sse2)() {
  if (!g_cpuIdInitialized) {
    FAR_FN(cpuid_init)();
  }
  return g_hasSSE2;
}

int FAR_FN(has_sse41)() {
  if (!g_cpuIdInitialized) {
    FAR_FN(cpuid_init)();
  }
  return g_hasSSE41;
}

int FAR_FN(has_sse42)() {
  if (!g_cpuIdInitialized) {
    FAR_FN(cpuid_init)();
  }
  return g_hasSSE42;
}

#ifdef __cplusplus
}  // namespace libfar
#endif  // __cplusplus
