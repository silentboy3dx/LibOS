#pragma once
#include "LibOS/desktop/Base.hpp"

namespace LibOS::Desktop {
    class Linux: public Base {
        public:
        virtual ~Linux() = default;
        WindowInfo GetActiveWindow() override;
    };
}
