cmake_minimum_required(VERSION 4.0)
project(LibOS VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(NOT (WINDOWS_PLATFORM OR X11_PLATFORM OR HYPRLAND_PLATFORM))
    message(FATAL_ERROR
            "No backend selected. Enable at least one of: "
            "WINDOWS_PLATFORM, X11_PLATFORM, HYPRLAND_PLATFORM."
    )
endif()

add_library(LibOS STATIC
        include/public/LibOS/LibOS.hpp

        include/public/LibOS/desktop/Linux.hpp
        include/public/LibOS/desktop/Base.hpp
        include/public/LibOS/desktop/Windows.hpp

        include/public/LibOS/system/Linux.hpp
        include/public/LibOS/system/Base.hpp
        include/public/LibOS/system/Windows.hpp

        src/LibOS/desktop/Linux.cpp
        src/LibOS/desktop/Windows.cpp
        src/LibOS/desktop/Base.cpp


        src/LibOS/system/Linux.cpp
        src/LibOS/system/Base.cpp

        src/library.cpp
)


if (WINDOWS_PLATFORM)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_WINDOWS)
    target_sources(${PROJECT_NAME} PRIVATE
            include/public/LibOS/desktop/Windows.hpp
            src/LibOS/system/Windows.cpp
    )
    include(Windows OPTIONAL)
endif()

if (X11_PLATFORM)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_X11)
    target_sources(${PROJECT_NAME} PRIVATE
            include/private/LibOS/desktop/Linux/X11.h
            src/LibOS/desktop/Linux/X11.cpp
    )
    include(Windows OPTIONAL)
endif()

if (HYPRLAND_PLATFORM)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_HYPRLAND)

    target_sources(${PROJECT_NAME} PRIVATE
            include/private/LibOS/desktop/Linux/Hyprland.h
            src/LibOS/desktop/Linux/Hyprland.cpp
    )

    include(HyprlandProtocols)

    target_sources(${PROJECT_NAME} PRIVATE
            ${HYPRLAND_PROTOCOL_SOURCES}
    )

    target_include_directories(${PROJECT_NAME} PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}/generated
    )
endif()



target_include_directories(LibOS
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/public>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/private>
)

if(WIN32)
    target_compile_definitions(LibOS PRIVATE PLATFORM_WINDOWS)
elseif(UNIX)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_compile_definitions(LibOS PRIVATE PLATFORM_LINUX)
    endif()
endif()

target_compile_definitions(LibOS PUBLIC LIBOS_EXPORTS)

if (X11_PLATFORM)
    include(X11)
endif()

include(Json)

# ============================================================
# Install rules
# ============================================================

include(GNUInstallDirs)

# Install headers
install(DIRECTORY include/public/LibOS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install library
install(TARGETS
        LibOS
        EXPORT LibOSTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Export targets
install(EXPORT LibOSTargets
        FILE LibOSTargets.cmake
        NAMESPACE LibOS::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LibOS
)

# Generate LibOSConfig.cmake
include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/LibOSConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/LibOSConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LibOS
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/LibOSConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LibOS
)
