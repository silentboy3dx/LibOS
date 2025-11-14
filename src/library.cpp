#include "LibOS/LibOS.hpp"

using namespace LibOS::Desktop;

namespace LibOS {
    std::optional<WindowInfo> GetActiveWindow() {
        auto result = DesktopInfoinstance()
        return result.GetActiveWindow();
    }

    Resolution GetResolution() {
        auto result = DesktopInfoinstance()
        return result.GetResolution();
    }

    SystemInfo GetSystemInfo() {
        auto result = SystemInfoinstance()
        return result.GetSystemInfo();
    }
}