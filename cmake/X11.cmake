if(CMAKE_SYSTEM_NAME STREQUAL "Linux" )
    message(STATUS "Configuring X11 (Linux Backend)")

    find_package(X11 REQUIRED)

    target_link_libraries(
            LibOS
            PRIVATE
                X11 Xrandr
    )
endif()
