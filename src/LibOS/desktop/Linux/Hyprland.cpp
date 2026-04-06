#include "LibOS/desktop/Linux/Hyprland.h"
#include "wlr-output-management-unstable-v1-client-protocol.h"

#include <nlohmann/json.hpp>
#include <wayland-client.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <optional>
#include <vector>

using namespace LibOS::Desktop;

// ------------------------------------------------------------
// Structs
// ------------------------------------------------------------

struct ModeInfo {
    int width = 0;
    int height = 0;
};

struct HeadInfo {
    zwlr_output_head_v1* head = nullptr;
    zwlr_output_mode_v1* currentMode = nullptr;
    ModeInfo mode;
    bool enabled = false;
};

struct WLData {
    zwlr_output_manager_v1* manager = nullptr;
    std::vector<HeadInfo*> heads;
    bool done = false;
};

// ------------------------------------------------------------
// Mode listener
// ------------------------------------------------------------

static void mode_size(void* data, zwlr_output_mode_v1*, int32_t w, int32_t h) {
    auto* head = static_cast<HeadInfo*>(data);
    head->mode.width = w;
    head->mode.height = h;
}

static void mode_refresh(void*, zwlr_output_mode_v1*, int32_t) {}
static void mode_preferred(void*, zwlr_output_mode_v1*) {}
static void mode_finished(void*, zwlr_output_mode_v1*) {}

static const zwlr_output_mode_v1_listener modeListener = {
    mode_size,
    mode_refresh,
    mode_preferred,
    mode_finished
};

// ------------------------------------------------------------
// Head listener (correct order)
// ------------------------------------------------------------

static void head_name(void*, zwlr_output_head_v1*, const char*) {}
static void head_description(void*, zwlr_output_head_v1*, const char*) {}
static void head_physical_size(void*, zwlr_output_head_v1*, int32_t, int32_t) {}

static void head_mode(void* data, zwlr_output_head_v1*, zwlr_output_mode_v1* mode) {
    auto* head = static_cast<HeadInfo*>(data);
    head->currentMode = mode;
    zwlr_output_mode_v1_add_listener(mode, &modeListener, head);
}

static void head_enabled(void* data, zwlr_output_head_v1*, int32_t enabled) {
    auto* head = static_cast<HeadInfo*>(data);
    head->enabled = enabled;
}

static void head_current_mode(void* data, zwlr_output_head_v1*, zwlr_output_mode_v1* mode) {
    auto* head = static_cast<HeadInfo*>(data);
    head->currentMode = mode;
    zwlr_output_mode_v1_add_listener(mode, &modeListener, head);
}

static void head_position(void*, zwlr_output_head_v1*, int32_t, int32_t) {}
static void head_transform(void*, zwlr_output_head_v1*, int32_t) {}
static void head_scale(void*, zwlr_output_head_v1*, wl_fixed_t) {}

static void head_done(void* data, zwlr_output_head_v1*) {
    auto* head = static_cast<HeadInfo*>(data);
    std::cerr << "[Hyprland] head_done for head " << head << std::endl;
}

static void head_finished(void*, zwlr_output_head_v1*) {}

static const zwlr_output_head_v1_listener headListener = {
    head_name,
    head_description,
    head_physical_size,
    head_mode,
    head_enabled,
    head_current_mode,
    head_position,
    head_transform,
    head_scale,
    head_done,
    head_finished
};

// ------------------------------------------------------------
// Manager listener
// ------------------------------------------------------------

static void manager_head(void* data, zwlr_output_manager_v1*, zwlr_output_head_v1* head) {
    auto* d = static_cast<WLData*>(data);
    auto* h = new HeadInfo{};
    h->head = head;
    d->heads.push_back(h);

    std::cerr << "[Hyprland] New head registered: " << h << std::endl;
    zwlr_output_head_v1_add_listener(head, &headListener, h);
}

static void manager_done(void* data, zwlr_output_manager_v1*, uint32_t) {
    auto* d = static_cast<WLData*>(data);
    d->done = true;
    std::cerr << "[Hyprland] Output manager done\n";
}

static void manager_finished(void*, zwlr_output_manager_v1*) {
    std::cerr << "[Hyprland] Output manager finished\n";
}

static const zwlr_output_manager_v1_listener managerListener = {
    manager_head,
    manager_done,
    manager_finished
};

// ------------------------------------------------------------
// Registry listener
// ------------------------------------------------------------

static void registry_add(void* data, wl_registry* reg, uint32_t name, const char* iface, uint32_t version) {
    auto* d = static_cast<WLData*>(data);

    if (strcmp(iface, zwlr_output_manager_v1_interface.name) == 0) {
        uint32_t v = std::min(version, 2u);
        std::cerr << "[Hyprland] Binding zwlr_output_manager_v1, version " << v << std::endl;
        d->manager = (zwlr_output_manager_v1*)wl_registry_bind(reg, name, &zwlr_output_manager_v1_interface, v);
        zwlr_output_manager_v1_add_listener(d->manager, &managerListener, d);
    }
}

static void registry_remove(void*, wl_registry*, uint32_t) {}

static const wl_registry_listener registryListener = {
    registry_add,
    registry_remove
};

// ------------------------------------------------------------
// Shared Hyprland IPC helper
// ------------------------------------------------------------

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
        std::cerr << "Could not create socket" << std::endl;
        return std::nullopt;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socketPath.c_str());

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Could not connect to socket" << std::endl;
        close(sock);
        return std::nullopt;
    }

    write(sock, command.c_str(), command.size());

    char buffer[8192];
    int len = read(sock, buffer, sizeof(buffer));
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

// ------------------------------------------------------------
// IPC: resolution
// ------------------------------------------------------------

static std::optional<Resolution> hyprland_ipc_resolution() {
    auto json = hyprland_ipc("monitors");
    if (!json || !json->is_array() || json->empty()) return std::nullopt;

    int w = (*json)[0].value("width", 0);
    int h = (*json)[0].value("height", 0);

    if (w == 0 || h == 0) return std::nullopt;

    return Resolution{w, h};
}

// ------------------------------------------------------------
// IPC: active window
// ------------------------------------------------------------

std::optional<WindowInfo> Hyprland::GetActiveWindow() {
    std::cerr << "[Hyprland] Getting active window via IPC\n";

    auto json = hyprland_ipc("activewindow");
    if (!json || !json->is_object()) {
        std::cerr << "[Hyprland] No active window\n";
        return std::nullopt;
    }

    WindowInfo info{};

    if (json->contains("title"))
        info.title = (*json)["title"].get<std::string>();
    if (json->contains("class"))
        info.className = (*json)["class"].get<std::string>();

    if (json->contains("at")) {
        info.x = (*json)["at"].value("x", 0);
        info.y = (*json)["at"].value("y", 0);
    }

    if (json->contains("size")) {
        info.width  = (*json)["size"].value("w", 0);
        info.height = (*json)["size"].value("h", 0);
    }

    std::cerr << "[Hyprland] Active window: " << info.title
              << " (" << info.className << ") "
              << info.width << "x" << info.height
              << " at " << info.x << "," << info.y << std::endl;

    return info;
}

// ------------------------------------------------------------
// Wayland: resolution via wlr-output-management
// ------------------------------------------------------------

static std::optional<Resolution> get_wlr_resolution() {
    wl_display* display = wl_display_connect(nullptr);
    if (!display) {
        std::cerr << "[Hyprland] wl_display_connect failed\n";
        return std::nullopt;
    }

    WLData data{};
    wl_registry* registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registryListener, &data);

    std::cerr << "Trying to get screen resolution" << std::endl;

    while (!data.done) {
        wl_display_roundtrip(display);
    }

    wl_display_disconnect(display);

    std::cout << "Got " << data.heads.size() << " heads" << std::endl;

    for (auto* h : data.heads) {
        if (h->enabled && h->mode.width > 0) {
            return Resolution{h->mode.width, h->mode.height};
        }
    }

    return std::nullopt;
}

// ------------------------------------------------------------
// Hyprland class
// ------------------------------------------------------------

Base& Hyprland::getInstance() {
    static Hyprland instance;
    return instance;
}

Resolution Hyprland::GetScreenResolution() {
    std::cerr << "Trying to get screen resolution" << std::endl;

    if (auto r = get_wlr_resolution()) return *r;
    if (auto r = hyprland_ipc_resolution()) return *r;

    std::cerr << "Could not determine screen resolution" << std::endl;
    return Resolution{0, 0};
}
