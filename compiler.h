//
// Created by novasurfer on 02/03/21.
//

#ifndef SC_BASE_COMPILER_H
#define SC_BASE_COMPILER_H

#if defined(__linux__)
#    define SC_OS_LINUX 1
#endif

#if defined(__APPLE__)
#    define SC_OS_APPLE 1
#endif

#if defined(_WIN32) || defined(_WIN64)
#    define SC_OS_WINDOWS 1
#endif

#if defined(__GNUC__)
#    define SC_COMPILER_GCC 1
#endif

#if defined(__clang__)
#    define SC_COMPILER_CLANG 1
#endif

#if defined(__GNUC__) || defined(__clang__)
#	define SC_COMPILER_GCC_OR_CLANG 1
#endif

#if defined(_MSC_VER)
#    define SC_COMPILER_MVC 1
#endif

#ifdef SC_COMPILER_MVC
#    define sc_forceinline __forceinline
#elif defined(SC_COMPILER_GCC)
#    define sc_forceinline inline __attribute__((__always_inline__))
#elif defined(SC_COMPILER_CLANG)
#    if __has_attribute(__always_inline__)
#        define sc_forceinline inline __attribute__((__always_inline__))
#    else
#        define sc_forceinline inline
#    endif
#else
#    define sc_forceinline inline
#endif

#endif //SC_BASE_COMPILER_H
