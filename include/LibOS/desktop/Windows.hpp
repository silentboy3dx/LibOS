#pragma once
#include "LibOS/desktop/Base.hpp"

#include <optional>

namespace LibOS::Desktop {


    class  Windows : public Base {
    public:
        ~Windows() override = default;
        static Windows &getInstance();

        std::optional<WindowInfo> GetActiveWindow() override;

    private:
        Windows() = default;
    };
}
