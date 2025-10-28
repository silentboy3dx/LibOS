#include "LibOS/desktop/Windows.hpp"

using namespace LibOS::Desktop;

#if PLATFORM_WINDOWS

std::optional<WindowInfo> Windows::GetActiveWindow() {
    return Base::GetActiveWindow();
}

#endif
