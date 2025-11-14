#include "LibOS/desktop/Base.hpp"


namespace LibOS::Desktop {
    std::optional<WindowInfo> Base::GetActiveWindow() {
        WindowInfo result = WindowInfo();
        return result;
    }

    Resolution Base::GetResolution() {
        return Resolution{0, 0};
    }
}
