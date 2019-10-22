#include "pch.h"

TEST(Memory, Alloc) {
  void* ptr = libfar::malloc_align(64, 16);
  ASSERT_NE(nullptr, ptr);
  ASSERT_EQ(0, (size_t)ptr & 15);
  libfar::free_align(ptr);
}

TEST(Memory, Alloc32) {
  byte32* ptr = libfar::malloc_align32(64);
  ASSERT_NE(nullptr, ptr);
  ASSERT_EQ(0, (size_t)ptr & 63);
  libfar::free_align32(ptr);
}