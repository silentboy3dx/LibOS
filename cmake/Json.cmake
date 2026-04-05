include(FetchContent)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)
set(nlohmann_json_DIR "${nlohmann_json_SOURCE_DIR}/cmake")

target_link_libraries(LibOS PRIVATE nlohmann_json::nlohmann_json)