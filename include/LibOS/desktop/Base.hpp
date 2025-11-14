#pragma once
#include <optional>

#include "WindowInfo.hpp"
#include "../system/SystemInfo.hpp"

namespace LibOS::Desktop {
    class Base {
    public:
        virtual ~Base() = default;
        virtual std::optional<WindowInfo> GetActiveWindow();
    };
}
