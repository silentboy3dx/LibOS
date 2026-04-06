#pragma once
#include "LibOS/desktop/Base.hpp"

#include <optional>

namespace LibOS::Desktop {
    class Linux : public Base {
    public:
        ~Linux() override = default;
        static Base &getInstance();

        std::optional<WindowInfo> GetActiveWindow() override;

        Resolution GetScreenResolution() override;

    private:
        Linux() = default;
    };
}
