find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(LibOS
        PUBLIC
        nlohmann_json::nlohmann_json
)
