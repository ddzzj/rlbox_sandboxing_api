#include <cstdint>

#include "test_include.hpp"

using rlbox::tainted;

// NOLINTNEXTLINE
TEST_CASE("Test operator + for numerics", "[operator]")
{
  tainted<int32_t, T_Sbx> a = 3;
  tainted<int32_t, T_Sbx> b = 3 + 4;
  tainted<int32_t, T_Sbx> c = a + 3;
  tainted<int32_t, T_Sbx> d = a + b;
  REQUIRE(a.UNSAFE_Unverified() == 3);
  REQUIRE(b.UNSAFE_Unverified() == 7);
  REQUIRE(c.UNSAFE_Unverified() == 6);
  REQUIRE(d.UNSAFE_Unverified() == 10);
}

// NOLINTNEXTLINE
TEST_CASE("Test operator + for pointers", "[operator]")
{
  T_Sbx sandbox;
  sandbox.create_sandbox();

  tainted<int32_t*, T_Sbx> pc = sandbox.malloc_in_sandbox<int>();
  tainted<int32_t*, T_Sbx> inc = pc + 1;

  auto diff = reinterpret_cast<char*>(inc.UNSAFE_Unverified()) - // NOLINT
              reinterpret_cast<char*>(pc.UNSAFE_Unverified());   // NOLINT
  REQUIRE(diff == 4);

  tainted<int32_t*, T_Sbx> nullPtr = nullptr;
  REQUIRE_THROWS(nullPtr + 1);

  REQUIRE_THROWS(pc + TestSandbox::SandboxMemorySize);

  //   tainted<int32_t*, T_Sbx> dec = inc - 1;
  //   REQUIRE(pc.UNSAFE_Unverified() == dec.UNSAFE_Unverified());

  sandbox.destroy_sandbox();
}