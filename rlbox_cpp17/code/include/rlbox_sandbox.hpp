#pragma once

#include <type_traits>

#include "rlbox_helpers.hpp"
#include "rlbox_typetraits.hpp"

namespace rlbox {

template<typename T_Sandbox>
class RLBoxSandbox : protected T_Sandbox
{
  KEEP_CLASSES_FRIENDLY

private:
  /***** Functions to adjust for custom machine models *****/

  template<class T>
  struct convert_sandbox_t_helper;

  template<>
  struct convert_sandbox_t_helper<void>
  {
    using type = void;
  };
  template<>
  struct convert_sandbox_t_helper<int>
  {
    using type = typename T_Sandbox::T_IntType;
  };
  template<>
  struct convert_sandbox_t_helper<unsigned int>
  {
    using type = std::make_unsigned_t<typename T_Sandbox::T_IntType>;
  };
  template<>
  struct convert_sandbox_t_helper<long>
  {
    using type = typename T_Sandbox::T_LongType;
  };
  template<>
  struct convert_sandbox_t_helper<unsigned long>
  {
    using type = std::make_unsigned_t<typename T_Sandbox::T_LongType>;
  };
  template<>
  struct convert_sandbox_t_helper<long long>
  {
    using type = typename T_Sandbox::T_LongLongType;
  };
  template<>
  struct convert_sandbox_t_helper<unsigned long long>
  {
    using type = std::make_unsigned_t<typename T_Sandbox::T_LongLongType>;
  };
  template<class T>
  struct convert_sandbox_t_helper<T*>
  {
    using type = typename T_Sandbox::T_PointerType;
  };
  template<class T, std::size_t N>
  struct convert_sandbox_t_helper<T[N]>
  {
    using type = typename convert_sandbox_t_helper<T>::type[N];
  };

  template<typename T>
  using convert_sandbox_t = typename convert_sandbox_t_helper<T>::type;
};

}