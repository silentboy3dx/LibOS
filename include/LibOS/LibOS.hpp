#pragma once

#include "LibOS/desktop/Base.hpp"
#include "LibOS/internals/export.hpp"
#include "LibOS/LibOS.hpp"

#if PLATFORM_WINDOWS
#   pragma message("✅ WINDOWS")
// #define MOUSE_CLASS() new ScreenshotWin32()
// #define KEYBOARD_CLASS() new ScreenshotWin32()
// #include "../../../src/windows/mouse.hpp"
// #include "../../../src/windows/keyboard.hpp
#include "LibIO/mouse/Windows.hpp"
#define MOUSE_CLASS() new LibIO::Mouse::Windows();
#elif PLATFORM_LINUX

    #include "desktop/Linux.hpp"
    #define DesktopInfoinstance() Desktop::Linux::getInstance();

//
// #include "../../../src/mouse/Linux.hpp"
// #include "../../../src/mouse/keyboard.hpp"
#else
// #   pragma message("✅ OOPS")
#define MouseControleInstance() nullptr
#define KeyboadControleInstance() nullptr
#endif

namespace LibOS {
    std::optional<Desktop::WindowInfo> LIBOS_API GetActiveWindow();
}
