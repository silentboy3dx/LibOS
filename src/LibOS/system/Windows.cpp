#include "LibOS/system/Linux.hpp"
#include "LibOS/system/SystemInfo.hpp"

#if PLATFORM_WINDOWS

using namespace LibOS::System;

namespace LibOS::System {

    Windows& Windows::getInstance() {
        static Windows instance;
        return instance;
    }

    SystemInfo Windows::GetSystemInfo() {
        SystemInfo info;

        // --- OS naam ---
        info.osName = "Windows";

        // --- Compiler ---
#if defined(__clang__)
        info.compiler = "Clang " + std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__);
#elif defined(__GNUC__)
        info.compiler = "GCC " + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__);
#elif defined(_MSC_VER)
        info.compiler = "MSVC " + std::to_string(_MSC_VER);
#else
        info.compiler = "Unknown Compiler";
#endif

        // --- C++ standaard ---
#if __cplusplus == 199711L
        info.cppVersion = "C++98";
#elif __cplusplus == 201103L
        info.cppVersion = "C++11";
#elif __cplusplus == 201402L
        info.cppVersion = "C++14";
#elif __cplusplus == 201703L
        info.cppVersion = "C++17";
#elif __cplusplus == 202002L
        info.cppVersion = "C++20";
#elif __cplusplus > 202002L
        info.cppVersion = "C++23 or later";
#else
        info.cppVersion = "Unknown C++ standard";
#endif

        // --- Runtime ---
        info.runtime = "MSVC STL";

        return info;
    }
}
#endif