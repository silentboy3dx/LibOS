#include "LibOS/desktop/Base.hpp"

#include <iostream>
#include <ostream>


namespace LibOS::Desktop {
    std::optional<WindowInfo> Base::GetActiveWindow() {
        WindowInfo result = WindowInfo();
        return result;
    }

    Resolution Base::GetScreenResolution() {
        std::cerr << "GetScreenResolution not implemented" << std::endl;
        return Resolution{0, 0};
    }
}
