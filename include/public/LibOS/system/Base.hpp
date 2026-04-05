#pragma once

#include <string>

namespace LibOS::System {

    struct SystemInfo {
        std::string osName;
        std::string compiler;
        std::string cppVersion;
        std::string runtime;
    };

    class Base {
    public:
        virtual ~Base() = default;
        virtual SystemInfo GetSystemInfo();
    };
}
