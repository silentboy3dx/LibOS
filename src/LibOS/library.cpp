#include "LibOS/internals/export.hpp"
#include "LibOS/LibOS.h"

using namespace LibOS::Desktop;

namespace LibOS {
    WindowInfo LIBOS_API GetActiveWindow() {
        auto &instance = LibOS::Desktop::Linux::getInstance();
        return instance.GetActiveWindow();
    }
}
