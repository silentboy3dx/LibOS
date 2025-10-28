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

        src/LibOS/desktop/Linux.cpp
        src/LibOS/desktop/Windows.cpp
        src/LibOS/desktop/Base.cpp
        src/library.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Platform detection
if(WIN32)
    message(STATUS "🖥️ Detected Windows platform")
    target_compile_definitions(${PROJECT_NAME} PUBLIC PLATFORM_WINDOWS)
elseif(UNIX)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        message(STATUS "🐧 Detected Linux platform")
        target_compile_definitions(${PROJECT_NAME} PUBLIC PLATFORM_LINUX)
    else()
        message(WARNING "⚠️ Unknown UNIX-like platform: ${CMAKE_SYSTEM_NAME}")
    endif()
else()
    message(WARNING "⚠️ Unknown platform: ${CMAKE_SYSTEM_NAME}")
endif()

include(X11)