#pragma once
#include "LibOS/desktop/Base.hpp"

#include <optional>

namespace LibOS::Desktop {
    class Linux : public Base {
    public:
        virtual ~Linux() = default;
        static Linux &getInstance();
        std::optional<WindowInfo> GetActiveWindow() override;
    private:
        Linux() = default;
    };
}
