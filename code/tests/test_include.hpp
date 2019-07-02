#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

// IWYU pragma: begin_exports
#include "catch2/catch.hpp"

#define RLBOX_NO_COMPILE_CHECKS
#define RLBOX_USE_EXCEPTIONS
#include "rlbox.hpp"

// IWYU pragma: end_exports

#define UNUSED(varName) (void)varName

class TestSandbox
{
private:
  std::pair<std::byte*, std::byte*> pow2SizeAlignedMalloc(size_t size)
  {
    size_t paddedSize = size * 2 + 1;
    std::byte* mem = new std::byte[paddedSize];
    uintptr_t memU = reinterpret_cast<uintptr_t>(mem);
    if ((memU & size) == 0) {
      return std::make_pair(mem, mem);
    }

    uintptr_t alignedMemU = memU;
    while ((alignedMemU & size) != 0) {
      alignedMemU++;
    }

    auto alignedMem = reinterpret_cast<std::byte*>(alignedMemU);
    return std::make_pair(mem, alignedMem);
  }

public:
  using T_LongLongType = int64_t;
  using T_LongType = int32_t;
  using T_IntType = int32_t;
  using T_PointerType = uint32_t;

  static const uint32_t SandboxMemorySize = 0xFF;
  static const uintptr_t SandboxMemoryBaseMask =
    ~(static_cast<uintptr_t>(SandboxMemorySize));
  uintptr_t UnalignedSandboxMemory;
  uintptr_t SandboxMemoryBase;

protected:
  template<typename... T_Args>
  inline void impl_create_sandbox(T_Args...)
  {
    auto unalignedAndAligned = pow2SizeAlignedMalloc(SandboxMemorySize);
    UnalignedSandboxMemory =
      reinterpret_cast<uintptr_t>(unalignedAndAligned.first);
    SandboxMemoryBase = reinterpret_cast<uintptr_t>(unalignedAndAligned.second);
  }

  inline void impl_destroy_sandbox()
  {
    delete[](reinterpret_cast<std::byte*>(UnalignedSandboxMemory));
  }

  template<typename T>
  inline void* impl_get_unsandboxed_pointer(T_PointerType p) const
  {
    return reinterpret_cast<void*>(SandboxMemoryBase +
                                   static_cast<uintptr_t>(p));
  }

  template<typename T>
  inline T_PointerType impl_get_sandboxed_pointer(const void* p) const
  {
    return static_cast<T_PointerType>(reinterpret_cast<uintptr_t>(p) -
                                      SandboxMemoryBase);
  }

  template<typename T>
  static inline void* impl_get_unsandboxed_pointer(
    T_PointerType p,
    const void* example_unsandboxed_ptr)
  {
    auto mask = SandboxMemoryBaseMask &
                reinterpret_cast<uintptr_t>(example_unsandboxed_ptr);
    return reinterpret_cast<void*>(mask + static_cast<uintptr_t>(p));
  }

  template<typename T>
  static inline T_PointerType impl_get_sandboxed_pointer(
    const void* p,
    const void* example_unsandboxed_ptr)
  {
    auto mask = SandboxMemoryBaseMask &
                reinterpret_cast<uintptr_t>(example_unsandboxed_ptr);
    return static_cast<T_PointerType>(reinterpret_cast<uintptr_t>(p) - mask);
  }

  inline T_PointerType impl_malloc_in_sandbox(size_t)
  {
    const auto random_address = 4;
    return static_cast<T_PointerType>(random_address);
  }

  inline void impl_free_in_sandbox(T_PointerType) {}

  static inline bool impl_is_in_same_sandbox(const void* p1, const void* p2)
  {
    auto mask1 = SandboxMemoryBaseMask & reinterpret_cast<uintptr_t>(p1);
    auto mask2 = SandboxMemoryBaseMask & reinterpret_cast<uintptr_t>(p2);
    return mask1 == mask2;
  }

  inline bool impl_is_pointer_in_sandbox_memory(const void* p)
  {
    auto mask = SandboxMemoryBaseMask & reinterpret_cast<uintptr_t>(p);
    return mask == SandboxMemoryBase;
  }
};

using T_Sbx = rlbox::RLBoxSandbox<TestSandbox>;