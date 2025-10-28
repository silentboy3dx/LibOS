#include "LibOS/desktop/Windows.hpp"

#if PLATFORM_WINDOWS
#include <windows.h>
#include <iostream>

using namespace LibOS::Desktop;

namespace LibOS::Desktop {

    Windows &Windows::getInstance() {
        static Windows instance;
        return instance;
    }

    std::optional<WindowInfo> Windows::GetActiveWindow() {
        HWND hwnd = GetForegroundWindow();

        char windowTitle[256];
        GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));

        if (strlen(windowTitle)) {
            WindowInfo result = WindowInfo();
            result.title = std::string(windowTitle);

            return result;
        }

        return std::nullopt;
    }
}

#endif
