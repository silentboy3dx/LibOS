#pragma once

#include "LibOS/desktop/Base.hpp"

#include <optional>

namespace LibOS::Desktop {
    class Hyprland : public Base {
    public:
        ~Hyprland() override = default;
        static Base &getInstance();

        std::optional<WindowInfo> GetActiveWindow() override;

        Resolution GetScreenResolution() override;

    private:
        Hyprland() = default;
    };
}
