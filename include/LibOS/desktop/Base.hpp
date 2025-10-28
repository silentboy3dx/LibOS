#pragma once
#include "WindowInfo.hpp"

namespace LibOS::Desktop {
    class Base {
    public:
        virtual ~Base() = default;
        virtual WindowInfo GetActiveWindow();
        static Base& getInstance();

        Base(const Base &) = delete;
        Base &operator=(const Base &) = delete;
        Base(Base &&) = delete;
        Base &operator=(Base &&) = delete;

    private:
        Base() = default;
    };
}
