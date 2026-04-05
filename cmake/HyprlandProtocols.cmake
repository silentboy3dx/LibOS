find_program(WAYLAND_SCANNER wayland-scanner REQUIRED)

set(PROTOCOL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protocols)
set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

file(MAKE_DIRECTORY ${GENERATED_DIR})

set(WLR_OUTPUT_XML ${PROTOCOL_DIR}/wlr-output-management-unstable-v1.xml)

set(WLR_OUTPUT_HEADER ${GENERATED_DIR}/wlr-output-management-unstable-v1-client-protocol.h)
set(WLR_OUTPUT_CODE   ${GENERATED_DIR}/wlr-output-management-unstable-v1-client-protocol.c)

add_custom_command(
        OUTPUT ${WLR_OUTPUT_HEADER}
        COMMAND ${WAYLAND_SCANNER} client-header
        ${WLR_OUTPUT_XML}
        ${WLR_OUTPUT_HEADER}
        DEPENDS ${WLR_OUTPUT_XML}
)

add_custom_command(
        OUTPUT ${WLR_OUTPUT_CODE}
        COMMAND ${WAYLAND_SCANNER} private-code
        ${WLR_OUTPUT_XML}
        ${WLR_OUTPUT_CODE}
        DEPENDS ${WLR_OUTPUT_XML}
)

set(HYPRLAND_PROTOCOL_SOURCES
        ${WLR_OUTPUT_HEADER}
        ${WLR_OUTPUT_CODE}
)
