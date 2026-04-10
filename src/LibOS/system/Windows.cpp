#include "LibOS/system/Windows.hpp"

#include <cstdlib>

#if PLATFORM_WINDOWS
namespace LibOS::System {
    Windows &Windows::getInstance() {
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

    std::optional<std::string> Windows::GetEnv(const std::string &key) {
        char *buffer = nullptr;
        size_t size = 0;

        if (_dupenv_s(&buffer, &size, key.c_str()) != 0 || buffer == nullptr)
            return std::nullopt;

        std::string value(buffer);
        free(buffer);
        return value;
    }

    bool Windows::PutEnv(const std::string &key, const std::string &value) {
        return _putenv_s(key.c_str(), value.c_str()) == 0;
    }
}
#endif
