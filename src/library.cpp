#include "LibOS/LibOS.hpp"

using namespace LibOS::Desktop;

namespace LibOS {
    std::optional<WindowInfo> GetActiveWindow() {
        auto result = DesktopInfoinstance()
        return result.GetActiveWindow();
    }

    Resolution GetScreenResolution() {
        auto result = DesktopInfoinstance()
        return result.GetScreenResolution();
    }

    SystemInfo GetSystemInfo() {
        auto result = SystemInfoinstance()
        return result.GetSystemInfo();
    }
}