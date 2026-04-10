#pragma once

#include "LibOS/system/Base.hpp"

namespace LibOS::System {
    class Windows : public Base {
    public:
        ~Windows() override = default;
        static Windows &getInstance();

        SystemInfo GetSystemInfo() override;
        std::optional<std::string> GetEnv(const std::string& key) override;
        bool PutEnv(const std::string& key, const std::string& value) override;

    private:
        Windows() = default;
    };
}