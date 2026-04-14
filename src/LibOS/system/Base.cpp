#include "LibOS/system/Base.hpp"


namespace LibOS::System {
    SystemInfo Base::GetSystemInfo() {
        return {};
    }

    std::optional<std::string> Base::GetEnv(const std::string& key) {
        return std::nullopt;
    }

    bool Base::PutEnv(const std::string &key, const std::string &value) {
        return false;
    }
}
