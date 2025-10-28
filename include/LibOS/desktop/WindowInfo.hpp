#pragma once

#include "LibOS/internals/export.hpp"
#include <string>

namespace LibOS::Desktop {
    struct WindowInfo {
        std::string title;
        ~WindowInfo() = default;
    };
}
