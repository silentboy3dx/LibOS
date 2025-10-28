#include "LibOS/desktop/Windows.hpp"

using namespace LibOS::Desktop;

#if PLATFORM_WINDOWS

WindowInfo Windows::GetActiveWindow() {
    return Base::GetActiveWindow();
}

#endif
