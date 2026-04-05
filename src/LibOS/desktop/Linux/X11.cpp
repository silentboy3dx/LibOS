#include "LibOS/desktop/Linux/X11.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>
#include <string>
#include <cstring>
#include <iostream>
#include <optional>

namespace LibOS::Desktop {
    Base &X11::getInstance() {
        static X11 instance;
        return instance;
    }

    std::optional<WindowInfo> X11::GetActiveWindow() {
        // Open X display
        Display *display = XOpenDisplay(nullptr);
        if (!display) {
            std::cerr << "Could not open X11 display" << std::endl;
            return std::nullopt;
        }

        std::optional<WindowInfo> result = std::nullopt;

        try {
            Window focusedWindow;
            int revertTo;

            // Get currently focused window
            XGetInputFocus(display, &focusedWindow, &revertTo);

            if (focusedWindow != None) {
                // Get the window title using XGetWMName
                XTextProperty textProperty;
                if (XGetWMName(display, focusedWindow, &textProperty) && textProperty.value) {
                    auto info = WindowInfo();
                    info.title = strdup(reinterpret_cast<char *>(textProperty.value));

                    XFree(textProperty.value);
                    result = info;
                }
            }
        } catch (const std::exception &e) {
            std::cerr << "Error checking window focus: " << e.what() << std::endl;
        }

        XCloseDisplay(display);
        return result;
    }

    Resolution X11::GetScreenResolution() {
        Display *display = XOpenDisplay(nullptr);
        if (!display) {
            throw std::runtime_error("Failed to open X11 display");
        }

        int screen = DefaultScreen(display);
        Window root = RootWindow(display, screen);

        XRRScreenResources *resources = XRRGetScreenResources(display, root);
        if (!resources) {
            XCloseDisplay(display);
            throw std::runtime_error("Failed to get screen resources");
        }

        XRRCrtcInfo *crtcInfo = nullptr;
        Resolution resolution{0,0};

        if (resources->ncrtc > 0) {
            RRCrtc crtc = resources->crtcs[0];
            crtcInfo = XRRGetCrtcInfo(display, resources, crtc);
            if (!crtcInfo) {
                XRRFreeScreenResources(resources);
                XCloseDisplay(display);
                throw std::runtime_error("Failed to get CRTC info");
            }

            resolution = Resolution{static_cast<int>(crtcInfo->width), static_cast<int>(crtcInfo->height)};
            XRRFreeCrtcInfo(crtcInfo);
        } else {
            XRRFreeScreenResources(resources);
            XCloseDisplay(display);
            throw std::runtime_error("No CRTCs found");
        }

        XRRFreeScreenResources(resources);
        XCloseDisplay(display);
        return resolution;
    }
}