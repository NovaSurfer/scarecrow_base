//
// Created by novasurfer on 11/30/20.
//
// -- Reference --
// GCC: https://github.com/gcc-mirror/gcc/blob/master/libstdc++-v3/include/std/type_traits
// LLVM: https://github.com/llvm/llvm-project/blob/master/libcxx/include/type_traits
// MICROSOFT: https://github.com/microsoft/STL/blob/master/stl/inc/type_traits

#ifndef SC_TYPETRAITS_H
#define SC_TYPETRAITS_H

namespace sc
{

    template <typename T, T value>
    struct integral_constant;

    template <bool Bool>
    using bool_constant = integral_constant<bool, Bool>;

    // Reference: https://clang.llvm.org/docs/LanguageExtensions.html#type-trait-primitives
    // The compiler intrinsics like "__is_is_trivially_constructible(T)" provide type information that is otherwise unavailable.
    template <typename T>
    struct is_trivial
        : bool_constant<__is_trivially_constructible(T) &&
                                      std::is_trivially_default_constructible<T>::value>
    {};

}

#endif //SC_TYPETRAITS_H
