#include "LibOS/LibOS.hpp"

using namespace LibOS;

namespace LibOS {
    std::optional<Desktop::WindowInfo> LIBOS_API GetActiveWindow() {
        auto &instance = DesktopInfoinstance();
        return instance.GetActiveWindow();
    }
}
