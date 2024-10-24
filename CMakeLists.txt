cmake_minimum_required(VERSION 3.24)

project(TapeSort
    LANGUAGES CXX
    VERSION 1.0.1
    HOMEPAGE_URL "https://github.com/Mistral5/thats-not-googled-by-others"
)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory("bin")
add_subdirectory("include")
add_subdirectory("src")

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Lib)

enable_testing()
add_subdirectory("tests")

# includes
set(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}Lib ${PROJECT_NAME}Tests)
foreach(target IN LISTS TARGETS)
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endforeach()
