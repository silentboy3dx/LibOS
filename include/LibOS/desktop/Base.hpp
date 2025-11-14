#pragma once

#include <optional>
#include <string>

namespace LibOS::Desktop {

    struct WindowInfo {
        std::string title;
        ~WindowInfo() = default;
    };

    struct Resolution {
        int width;
        int height;
    };

    class Base {
    public:
        virtual ~Base() = default;
        virtual std::optional<WindowInfo> GetActiveWindow();
        virtual Resolution GetResolution();
    };
}
