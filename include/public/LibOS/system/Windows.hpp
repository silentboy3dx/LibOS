#pragma once

#include "LibOS/system/Base.hpp"

namespace LibOS::System {
    class Windows : public Base {
    public:
        ~Windows() override = default;
        static Windows &getInstance();

        SystemInfo GetSystemInfo() override;

    private:
        Windows() = default;
    };
}