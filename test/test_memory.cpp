#include "pch.h"

TEST(Memory, Alloc) {
  for (int size = 1; size <= 256; ++size) {
    for (int align_exp = 2; align_exp <= 5; ++align_exp) {
      const size_t align = (1 << align_exp);
      void* ptr = libfar::malloc_align(size, align);
      ASSERT_NE(nullptr, ptr);
      ASSERT_EQ(0, (size_t)ptr & (align - 1));
      libfar::free_align(ptr);
    }
  }
}

TEST(Memory, Alloc16) {
  for (int size = 1; size <= 256; ++size) {
    byte16* ptr = libfar::malloc_align16(size);
    ASSERT_NE(nullptr, ptr);
    ASSERT_EQ(0, (size_t)ptr & 15);
    libfar::free_align16(ptr);
  }
}

TEST(Memory, Alloc32) {
  for (int size = 1; size <= 256; ++size) {
    byte32* ptr = libfar::malloc_align32(size);
    ASSERT_NE(nullptr, ptr);
    ASSERT_EQ(0, (size_t)ptr & 31);
    libfar::free_align32(ptr);
  }
}

TEST(Memory, Alloc64) {
  for (int size = 1; size <= 256; ++size) {
    byte64* ptr = libfar::malloc_align64(size);
    ASSERT_NE(nullptr, ptr);
    ASSERT_EQ(0, (size_t)ptr & 63);
    libfar::free_align64(ptr);
  }
}

TEST(Memory, AllocTemplate) {
  for (int size = 1; size <= 256; ++size) {
    auto ptr = libfar::malloc_align<short128>(size);
    ASSERT_NE(nullptr, ptr);
    ASSERT_EQ(0, (size_t)ptr & 15);
    libfar::free_align(ptr);
  }
}
