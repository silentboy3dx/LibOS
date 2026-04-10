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

    std::optional<std::string> GetEnv(const std::string& key) {
        return SystemInfoinstance().GetEnv(key);
    }

    bool PutEnv(const std::string& key, const std::string& value) {
        return SystemInfoinstance().PutEnv(key, value);
    }
}