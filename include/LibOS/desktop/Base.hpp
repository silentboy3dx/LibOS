#pragma once
#include <optional>

#include "WindowInfo.hpp"

namespace LibOS::Desktop {
    class Base {
    public:
        virtual ~Base() = default;
        virtual std::optional<WindowInfo> GetActiveWindow();
    };
}
