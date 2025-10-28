#include "LibOS/desktop/Linux.hpp"


#if PLATFORM_LINUX
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <optional>

using namespace LibOS::Desktop;

namespace LibOS::Desktop {

    Linux& Linux::getInstance() {
        static Linux instance;
        return instance;
    }

    std::optional<WindowInfo> Linux::GetActiveWindow() {
        // Open X display
        Display *display = XOpenDisplay(nullptr);
        if (!display) {
            std::cerr << "Could not open X11 display" << std::endl;
            return std::nullopt;
        }

        std::optional<WindowInfo> result = std::nullopt;

        try {
            Window focusedWindow;
            int revertTo;

            // Get currently focused window
            XGetInputFocus(display, &focusedWindow, &revertTo);

            if (focusedWindow != None) {
                // Get the window title using XGetWMName
                XTextProperty textProperty;
                if (XGetWMName(display, focusedWindow, &textProperty) && textProperty.value) {

                    auto info = WindowInfo();
                    info.title = strdup(reinterpret_cast<char*>(textProperty.value));

                    XFree(textProperty.value);
                    result = info;
                }
            }
        } catch (const std::exception &e) {
            std::cerr << "Error checking window focus: " << e.what() << std::endl;
        }

        XCloseDisplay(display);
        return result;
    }
}
#endif
