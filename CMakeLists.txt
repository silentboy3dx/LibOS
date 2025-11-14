cmake_minimum_required(VERSION 4.0)
project(LibOS)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_library(${PROJECT_NAME} STATIC
        include/LibOS/LibOS.hpp

        include/LibOS/desktop/Linux.hpp
        include/LibOS/desktop/Base.hpp
        include/LibOS/desktop/Windows.hpp
        include/LibOS/desktop/WindowInfo.hpp

        include/LibOS/system/Linux.hpp
        include/LibOS/system/Base.hpp
        include/LibOS/system/Windows.hpp
        include/LibOS/system/SystemInfo.hpp

        src/LibOS/desktop/Linux.cpp
        src/LibOS/desktop/Windows.cpp
        src/LibOS/desktop/Base.cpp

        src/LibOS/system/Windows.cpp
        src/LibOS/system/Linux.cpp

        src/library.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
elseif(UNIX)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
    endif()
endif()

target_compile_definitions(LibOS PRIVATE LIBOS_EXPORTS)

include(X11)