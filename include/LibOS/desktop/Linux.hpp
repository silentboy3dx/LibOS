#pragma once
#include "LibOS/desktop/Base.hpp"

#include <optional>

namespace LibOS::Desktop {
    class Linux : public Base {
    public:
        ~Linux() override = default;
        static Linux &getInstance();

        std::optional<WindowInfo> GetActiveWindow() override;

        Resolution GetResolution() override;

    private:
        Linux() = default;
    };
}
