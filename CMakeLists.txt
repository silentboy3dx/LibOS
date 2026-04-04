cmake_minimum_required(VERSION 4.0)
project(LibOS VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_library(LibOS STATIC
        include/LibOS/LibOS.hpp

        include/LibOS/desktop/Linux.hpp
        include/LibOS/desktop/Base.hpp
        include/LibOS/desktop/Windows.hpp

        include/LibOS/system/Linux.hpp
        include/LibOS/system/Base.hpp
        include/LibOS/system/Windows.hpp

        src/LibOS/desktop/Linux.cpp
        src/LibOS/desktop/Windows.cpp
        src/LibOS/desktop/Base.cpp

        src/LibOS/system/Windows.cpp
        src/LibOS/system/Linux.cpp
        src/LibOS/system/Base.cpp

        src/library.cpp
)

target_include_directories(LibOS PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(WIN32)
    target_compile_definitions(LibOS PRIVATE PLATFORM_WINDOWS)
elseif(UNIX)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_compile_definitions(LibOS PRIVATE PLATFORM_LINUX)
    endif()
endif()

target_compile_definitions(LibOS PUBLIC LIBOS_EXPORTS)

include(X11)

# ============================================================
# Install rules
# ============================================================

include(GNUInstallDirs)

# Install headers
install(DIRECTORY include/LibOS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install library
install(TARGETS LibOS
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
