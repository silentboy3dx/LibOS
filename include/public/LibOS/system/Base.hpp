#pragma once

#include <string>
#include <optional>

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
        virtual std::optional<std::string> GetEnv(const std::string &key);
        virtual bool PutEnv(const std::string &key, const std::string &value);
    };
}
