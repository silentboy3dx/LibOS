#include "LibOS/desktop/Windows.hpp"

using namespace LibOS::Desktop;

#if PLATFORM_WINDOWS

#include <windows.h>
#include <iostream>

std::optional<WindowInfo> Windows::GetActiveWindow() {
    HWND hwnd = GetForegroundWindow();

    // Buffer voor de venstertitel
    char windowTitle[256];
    GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));

    if (strlen(windowTitle)) {
        WindowInfo result = WindowInfo();
        result.title = std::string(windowTitle);

        return result;
    }
    // return builder.ToString().Contains(windowTitle, StringComparison.OrdinalIgnoreCase);

    return std::nullopt;
}

#endif
