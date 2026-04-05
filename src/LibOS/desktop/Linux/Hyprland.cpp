#include "LibOS/desktop/Linux/Hyprland.h"
#include <nlohmann/json.hpp>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <optional>
#include <ostream>
#include <wayland-client.h>

using namespace LibOS::Desktop;

struct WLData {
    wl_output* output = nullptr;
    int width = 0;
    int height = 0;
};

static void wl_geometry(void*, wl_output*, int32_t, int32_t, int32_t, int32_t, int32_t, const char*, const char*, int32_t) {}
static void wl_mode(void* data, wl_output*, uint32_t flags, int32_t w, int32_t h, int32_t) {
    if (flags & WL_OUTPUT_MODE_CURRENT) {
        WLData* d = static_cast<WLData*>(data);
        d->width = w;
        d->height = h;
    }
}
static void wl_done(void*, wl_output*) {}
static void wl_scale(void*, wl_output*, int32_t) {}

static const wl_output_listener outputListener = {
    wl_geometry,
    wl_mode,
    wl_done,
    wl_scale
};

std::optional<Resolution> get_wayland_resolution() {
    wl_display* display = wl_display_connect(nullptr);
    if (!display) return std::nullopt;

    wl_registry* registry = wl_display_get_registry(display);
    WLData data{};

    static const wl_registry_listener registryListener = {
        [](void* d, wl_registry* reg, uint32_t name, const char* iface, uint32_t) {
            WLData* data = static_cast<WLData*>(d);
            if (strcmp(iface, wl_output_interface.name) == 0) {
                data->output = (wl_output*)wl_registry_bind(reg, name, &wl_output_interface, 4);
                wl_output_add_listener(data->output, &outputListener, data);
            }
        },
        [](void*, wl_registry*, uint32_t) {}
    };

    wl_registry_add_listener(registry, &registryListener, &data);
    wl_display_roundtrip(display);
    wl_display_disconnect(display);

    if (data.width == 0) return std::nullopt;
    return Resolution{data.width, data.height};
}


Base& Hyprland::getInstance() {
    static Hyprland instance;
    return instance;
}

std::optional<WindowInfo> Hyprland::GetActiveWindow() {
    const char* sig = getenv("HYPRLAND_INSTANCE_SIGNATURE");
    const char* runtime = getenv("XDG_RUNTIME_DIR");
    if (!sig || !runtime) return std::nullopt;

    std::string socketPath = std::string(runtime) + "/hypr/" + sig + "/.socket2.sock";

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return std::nullopt;
    }
    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socketPath.c_str());

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Failed to connect to socket" << std::endl;
        close(sock);
        return std::nullopt;
    }

    const char* cmd = "activewindow";
    write(sock, cmd, strlen(cmd));

    char buffer[8192];
    int len = read(sock, buffer, sizeof(buffer));
    close(sock);

    if (len <= 0) return std::nullopt;

    auto json = nlohmann::json::parse(std::string(buffer, len));

    std::cout << "json: " << json << std::endl;

    WindowInfo info;
    info.title = json.value("title", "");

    return info.title.empty() ? std::nullopt : std::optional(info);
}

Resolution Hyprland::GetScreenResolution() {
    auto res = get_wayland_resolution();
    if (res.has_value()) return res.value();
    return Resolution{0, 0};
}
