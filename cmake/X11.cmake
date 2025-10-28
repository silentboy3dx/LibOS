if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT WAYLAND)
    message(STATUS "Configuring X11 (Linux Backend)")

    find_package(X11 REQUIRED)

    target_link_libraries(${PROJECT_NAME} PRIVATE X11 Xrandr)
endif()
