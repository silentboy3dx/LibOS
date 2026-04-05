#pragma once

#include "LibOS/desktop/Base.hpp"

#include <optional>

namespace LibOS::Desktop {
    class X11 : public Base {
    public:
        ~X11() override = default;
        static Base &getInstance();

        std::optional<WindowInfo> GetActiveWindow() override;

        Resolution GetScreenResolution() override;

    private:
        X11() = default;
    };
}
