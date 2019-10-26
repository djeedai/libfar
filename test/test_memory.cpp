#include "pch.h"

TEST(Memory, Alloc) {
  const size_t align = (1 << 4);  // 16
  void* ptr = libfar::malloc_align(64, align);
  ASSERT_NE(nullptr, ptr);
  ASSERT_EQ(0, (size_t)ptr & (align - 1));
  libfar::free_align(ptr);
}

TEST(Memory, Alloc32) {
  byte32* ptr = libfar::malloc_align32(64);
  ASSERT_NE(nullptr, ptr);
  ASSERT_EQ(0, (size_t)ptr & 31);
  libfar::free_align32(ptr);
}
