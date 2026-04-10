#include "LibOS/system/Linux.hpp"

#include <cstdlib>

#if PLATFORM_LINUX
#include <sys/utsname.h>

using namespace LibOS::System;

namespace LibOS::System {

    Linux& Linux::getInstance() {
        static Linux instance;
        return instance;
    }

    SystemInfo Linux::GetSystemInfo() {
        SystemInfo info;

        // --- OS naam ---
        struct utsname buffer;
        if (uname(&buffer) == 0) {
            info.osName = buffer.sysname;   // meestal "Linux"
        } else {
            info.osName = "Linux";
        }

        // --- Compiler ---
#if defined(__clang__)
        info.compiler = "Clang " + std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__);
#elif defined(__GNUC__)
        info.compiler = "GCC " + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__);
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
        info.runtime = "libstdc++ / libc++";

        return info;
    }

    std::optional<std::string> Windows::GetEnv(const std::string &key) {
        const char* value = std::getenv(key.c_str());
        if (!value)
            return std::nullopt;

        return std::string(value);
    }

    bool Windows::PutEnv(const std::string &key, const std::string &value) {
        std::string entry = key + "=" + value;

        char* env = new char[entry.size() + 1];
        std::copy(entry.begin(), entry.end(), env);
        env[entry.size()] = '\0';

        return putenv(env) == 0;
    }
}
#endif