include(FetchContent)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

set(SOURCES
    config_reader.cpp
    driver_params.cpp
    tape.cpp
    driver.cpp
    driver_perf_stats.cpp
    tape_generator.cpp
)

add_library(${PROJECT_NAME}Lib SHARED ${SOURCES} ${INCLUDES})
target_link_libraries(${PROJECT_NAME}Lib PUBLIC nlohmann_json::nlohmann_json)
