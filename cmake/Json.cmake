find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(LibOS PRIVATE nlohmann_json::nlohmann_json)
