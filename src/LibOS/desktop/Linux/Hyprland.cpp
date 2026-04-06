#include <cstdio>
#include <cstdlib>
#include <memory>
#include <array>
#include <string>
#include <optional>
#include <iostream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace LibOS {

struct Resolution {
    int width{};
    int height{};
};

struct ActiveWindow {
    std::string appClass;
    std::string title;
};

class Hyprland {
public:
    static Resolution GetScreenResolution();
    static ActiveWindow GetActiveWindow();

private:
    static std::optional<json> runHyprctlJson(const std::string& subcommand);
};

// ------------------------------------------------------------
// Helper: run "hyprctl -j <subcommand>" and parse JSON
// ------------------------------------------------------------

std::optional<json> Hyprland::runHyprctlJson(const std::string& subcommand) {
    std::string cmd = "hyprctl -j " + subcommand + " 2>/dev/null";

    std::array<char, 4096> buffer{};
    std::string output;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        std::cerr << "[Hyprland] Failed to run: " << cmd << "\n";
        return std::nullopt;
    }

    while (true) {
        std::size_t n = fread(buffer.data(), 1, buffer.size(), pipe);
        if (n == 0) break;
        output.append(buffer.data(), n);
    }

    int rc = pclose(pipe);
    if (rc != 0 || output.empty()) {
        std::cerr << "[Hyprland] hyprctl returned non‑zero or empty output for: "
                  << subcommand << "\n";
        return std::nullopt;
    }

    try {
        return json::parse(output);
    } catch (const std::exception& e) {
        std::cerr << "[Hyprland] JSON parse error for '" << subcommand
                  << "': " << e.what() << "\n";
        return std::nullopt;
    }
}

// ------------------------------------------------------------
// Resolution via: hyprctl -j monitors
// ------------------------------------------------------------

Resolution Hyprland::GetScreenResolution() {
    std::cerr << "[Hyprland] GetScreenResolution via hyprctl -j monitors\n";

    auto j = runHyprctlJson("monitors");
    if (!j || !j->is_array() || j->empty()) {
        std::cerr << "[Hyprland] monitors JSON invalid or empty\n";
        return Resolution{0, 0};
    }

    const auto& primary = (*j)[0];

    if (!primary.contains("width") || !primary.contains("height")) {
        std::cerr << "[Hyprland] monitors JSON missing width/height\n";
        return Resolution{0, 0};
    }

    int w = 0;
    int h = 0;

    try {
        w = primary.at("width").get<int>();
        h = primary.at("height").get<int>();
    } catch (const std::exception& e) {
        std::cerr << "[Hyprland] monitors width/height parse error: "
                  << e.what() << "\n";
        return Resolution{0, 0};
    }

    return Resolution{w, h};
}

// ------------------------------------------------------------
// Active window via: hyprctl -j activewindow
// ------------------------------------------------------------

ActiveWindow Hyprland::GetActiveWindow() {
    std::cerr << "[Hyprland] GetActiveWindow via hyprctl -j activewindow\n";

    auto j = runHyprctlJson("activewindow");
    if (!j || !j->is_object()) {
        std::cerr << "[Hyprland] activewindow JSON invalid\n";
        return ActiveWindow{"", ""};
    }

    std::string cls;
    std::string title;

    try {
        if (j->contains("class"))
            cls = j->at("class").get<std::string>();
        if (j->contains("title"))
            title = j->at("title").get<std::string>();
    } catch (const std::exception& e) {
        std::cerr << "[Hyprland] activewindow parse error: "
                  << e.what() << "\n";
        return ActiveWindow{"", ""};
    }

    return ActiveWindow{cls, title};
}

} // namespace LibOS
