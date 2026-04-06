#include "LibOS/desktop/Linux.hpp"

#if PLATFORM_LINUX
#include <iostream>
#include <optional>

#include "LibOS/desktop/Linux/Hyprland.h"


#if USE_X11
#include "LibOS/desktop/Linux/X11.h"
#endif

#if USE_HYPRLAND
#include "LibOS/desktop/Linux/Hyprland.h"
#endif

using namespace LibOS::Desktop;

namespace LibOS::Desktop {
    Base &Linux::getInstance() {
#if USE_X11
        return X11::getInstance();
#elif USE_HYPRLAND
        std::cout << "Hyprland backend selected!" << std::endl;
        return Hyprland::getInstance();
#else
#   error "No Linux backend selected! Define USE_X11, USE_HYPRLAND or USE_WAYLAND."
#endif
    }

    std::optional<WindowInfo> Linux::GetActiveWindow() {
        return getInstance().GetActiveWindow();
    }

    Resolution Linux::GetScreenResolution() {
        return getInstance().GetScreenResolution();
    }
}
#endif
