#include "LibOS/desktop/Base.hpp"


namespace LibOS::Desktop {
    WindowInfo Base::GetActiveWindow() {
        WindowInfo result = WindowInfo();
        return result;
    }

    Base& Base::getInstance() {
        static Base instance;
        return instance;
    }
}
