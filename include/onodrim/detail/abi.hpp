#pragma once

#if defined(_MSC_VER)
#if _MSVC_LANG < 202002L
#error "ONODRIM C++ version error"
#include "ONODRIM requires C++20 or higher. Use /std:c++20 or higher to enable C++20 features."
#endif
#else
#if __cplusplus < 202002L
#error "ONODRIM C++ version error"
#include "ONODRIM requires C++20 or higher. Use -std=c++20 or higher to enable C++20 features."
#endif
#endif

// Macro to replace std::forward. Better compile-time + less error-prone
#define ONODRIM_FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)