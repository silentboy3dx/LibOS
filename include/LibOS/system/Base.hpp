#pragma once

#include "LibOS/system/SystemInfo.hpp"

namespace LibOS::System {
    class Base {
    public:
        virtual ~Base() = default;
        virtual SystemInfo GetSystemInfo();
    };
}
