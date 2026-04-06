#include "LibOS/LibOS.hpp"

using namespace LibOS::Desktop;

namespace LibOS {
    std::optional<WindowInfo> GetActiveWindow() {
        return DesktopInfoinstance().GetActiveWindow();
    }

    Resolution GetScreenResolution() {
        return DesktopInfoinstance().GetScreenResolution();
    }

    SystemInfo GetSystemInfo() {
        return SystemInfoinstance().GetSystemInfo();
    }
}