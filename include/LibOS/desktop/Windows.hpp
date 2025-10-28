#pragma once
#include "LibOS/desktop/Base.hpp"

namespace LibOS::Desktop {
    class Windows : public Base {
    public:
        virtual ~Windows() = default;
        WindowInfo GetActiveWindow() override;
    };
}
