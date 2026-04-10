#pragma once

#include "LibOS/desktop/Base.hpp"
#include "LibOS/system/Base.hpp"

#if USE_WINDOWS
    #include "desktop/Windows.hpp"
    #include "system/Windows.hpp"
    #define DesktopInfoinstance() Desktop::Windows::getInstance()
    #define SystemInfoinstance() System::Windows::getInstance()

#else

    #include "desktop/Linux.hpp"
    #include "system/Linux.hpp"
    #define DesktopInfoinstance() Desktop::Linux::getInstance()
    #define SystemInfoinstance() System::Linux::getInstance()

#endif

using namespace LibOS::Desktop;
using namespace LibOS::System;

namespace LibOS {
    std::optional<WindowInfo> GetActiveWindow();
    Resolution GetScreenResolution();
    SystemInfo GetSystemInfo();

    std::optional<std::string> GetEnv(const std::string& key);
    bool PutEnv(const std::string& key, const std::string& value);
}