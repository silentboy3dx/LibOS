#include <cstdio>
#include <cstdlib>
#include <array>
#include <string>
#include <optional>

#include <nlohmann/json.hpp>
#include "LibOS/desktop/Linux/Hyprland.h"

using json = nlohmann::json;

namespace {

    std::optional<json> runHyprctlJson(const std::string& subcommand) {
        std::string cmd = "hyprctl -j " + subcommand + " 2>/dev/null";

        std::array<char, 4096> buffer{};
        std::string output;

        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe)
            return std::nullopt;

        while (true) {
            std::size_t n = fread(buffer.data(), 1, buffer.size(), pipe);
            if (n == 0) break;
            output.append(buffer.data(), n);
        }

        int rc = pclose(pipe);
        if (rc != 0 || output.empty())
            return std::nullopt;

        try {
            return json::parse(output);
        } catch (...) {
            return std::nullopt;
        }
    }

} // namespace

namespace LibOS::Desktop {

    Base& Hyprland::getInstance() {
        static Hyprland instance;
        return instance;
    }

    Resolution Hyprland::GetScreenResolution() {
        auto j = runHyprctlJson("monitors");
        if (!j || !j->is_array() || j->empty())
            return Resolution{0, 0};

        const auto& m = (*j)[0];
        if (!m.contains("width") || !m.contains("height"))
            return Resolution{0, 0};

        return Resolution{
            m.at("width").get<int>(),
            m.at("height").get<int>()
        };
    }

    std::optional<WindowInfo> Hyprland::GetActiveWindow() {
        auto j = runHyprctlJson("activewindow");
        if (!j || !j->is_object())
            return std::nullopt;

        WindowInfo info;

        if (j->contains("title"))
            info.title = j->at("title").get<std::string>();
        else
            info.title.clear();

        return info;
    }

}