#pragma once

#include "LibOS/internals/export.hpp"
#include <string>

namespace LibOS::Desktop {
    struct LIBOS_API WindowInfo {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
        std::string title;
    };
}
