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
        virtual ~Base();

        virtual std::optional<WindowInfo> GetActiveWindow() = 0;
        virtual Resolution GetScreenResolution() = 0;
    };
}
