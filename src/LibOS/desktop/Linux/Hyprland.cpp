#include "LibOS/desktop/Linux/Hyprland.h"

#include <nlohmann/json.hpp>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <optional>

using namespace LibOS::Desktop;

static std::optional<nlohmann::json> hyprland_ipc(const std::string& command) {
    const char* sig = getenv("HYPRLAND_INSTANCE_SIGNATURE");
    const char* runtime = getenv("XDG_RUNTIME_DIR");
    if (!sig || !runtime) {
        std::cerr << "[Hyprland] HYPRLAND_INSTANCE_SIGNATURE or XDG_RUNTIME_DIR not set\n";
        return std::nullopt;
    }

    std::string socketPath = std::string(runtime) + "/hypr/" + sig + "/.socket2.sock";
    std::cout << "Trying to connect to " << socketPath << std::endl;

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Could not create socket\n";
        return std::nullopt;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socketPath.c_str());

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Could not connect to socket\n";
        close(sock);
        return std::nullopt;
    }

    write(sock, command.c_str(), command.size());


    char buffer[8192];
    int len = read(sock, buffer, sizeof(buffer));

    std::cout << "[Hyprland] IPC response: " << std::string(buffer, len) << std::endl;
    close(sock);

    if (len <= 0) {
        std::cerr << "[Hyprland] Empty IPC response\n";
        return std::nullopt;
    }

    auto json = nlohmann::json::parse(std::string(buffer, len), nullptr, false);
    if (json.is_discarded()) {
        std::cerr << "[Hyprland] JSON parse error\n";
        return std::nullopt;
    }

    std::cout << json << std::endl;
    return json;
}

static std::optional<Resolution> hyprland_ipc_resolution() {
    auto json = hyprland_ipc("monitors");
    if (!json || !json->is_array() || json->empty()) return std::nullopt;

    int w = (*json)[0].value("width", 0);
    int h = (*json)[0].value("height", 0);

    if (w == 0 || h == 0) return std::nullopt;

    return Resolution{w, h};
}

std::optional<WindowInfo> Hyprland::GetActiveWindow() {
    std::cerr << "[Hyprland] Getting active window via IPC\n";

    auto json = hyprland_ipc("activewindow");
    if (!json || !json->is_object()) {
        std::cerr << "[Hyprland] No active window\n";
        return std::nullopt;
    }

    WindowInfo info{};
    info.title = (*json).value("title", "");

    std::cerr << "[Hyprland] Active window title: " << info.title << std::endl;
    return info;
}

Base& Hyprland::getInstance() {
    static Hyprland instance;
    return instance;
}

Resolution Hyprland::GetScreenResolution() {
    std::cerr << "Trying to get screen resolution\n";

    if (auto r = hyprland_ipc_resolution()) return *r;

    std::cerr << "Could not determine screen resolution\n";
    return Resolution{0, 0};
}
