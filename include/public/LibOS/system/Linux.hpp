#pragma once
#include "LibOS/system/Base.hpp"
#include <optional>

namespace LibOS::System {
    class Linux : public Base {
    public:
        ~Linux() override = default;
        static Linux &getInstance();

        SystemInfo GetSystemInfo() override;
        std::optional<std::string> GetEnv(const std::string &key) override;
        bool PutEnv(const std::string &key, const std::string &value) override;

    private:
        Linux() = default;
    };
}
