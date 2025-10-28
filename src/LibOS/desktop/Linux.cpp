#include "LibOS/desktop/Linux.hpp"


#if PLATFORM_LINUX

#include <X11/Xlib.h>

using namespace LibOS::Desktop;

WindowInfo Linux::GetActiveWindow() {
    auto result = WindowInfo();
    return result;
}

#endif
