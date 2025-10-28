#pragma once

#include "LibOS/desktop/Base.hpp"
#include "LibOS/internals/export.hpp"
#include "LibOS/LibOS.hpp"

#if PLATFORM_WINDOWS
// #   pragma message("✅ WINDOWS")
    #include "desktop/Windows.hpp"
    #define DesktopInfoinstance() Desktop::Windows::getInstance();

#elif PLATFORM_LINUX

    #include "desktop/Linux.hpp"
    #define DesktopInfoinstance() Desktop::Linux::getInstance();

#else

// #   pragma message("✅ OOPS")
#define DesktopInfoinstance() nullptr

#endif

using namespace LibOS::Desktop;

namespace LibOS {
    std::optional<WindowInfo> GetActiveWindow();
}