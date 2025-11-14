#pragma once

#include "LibOS/desktop/Base.hpp"
#include "LibOS/system/Base.hpp"
#include "LibOS/internals/export.hpp"


#if PLATFORM_WINDOWS
// #   pragma message("✅ WINDOWS")
    #include "desktop/Windows.hpp"
    #include "system/Windows.hpp"
    #define DesktopInfoinstance() Desktop::Windows::getInstance();
    #define SystemInfoinstance() System::Windows::getInstance();

#elif PLATFORM_LINUX

    #include "desktop/Linux.hpp"
    #include "system/Linux.hpp"
    #define DesktopInfoinstance() Desktop::Linux::getInstance();
    #define SystemInfoinstance() System::Linux::getInstance();


#else

// #   pragma message("✅ OOPS")
#define DesktopInfoinstance() nullptr
#define SystemInfoinstance() nullptr

#endif

using namespace LibOS::Desktop;
using namespace LibOS::System;

namespace LibOS {
    std::optional<WindowInfo> GetActiveWindow();
    Resolution GetScreenResolution();
    SystemInfo GetSystemInfo();
}