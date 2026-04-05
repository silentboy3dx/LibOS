#pragma once
#include "LibOS/system/Base.hpp"


namespace LibOS::System {
    class Linux : public Base {
    public:
        ~Linux() override = default;
        static Linux &getInstance();

        SystemInfo GetSystemInfo() override;

    private:
        Linux() = default;
    };
}
