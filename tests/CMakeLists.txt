include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    ${PROJECT_NAME}Tests
    tests.cpp
)

target_link_libraries(
    ${PROJECT_NAME}Tests PRIVATE
    GTest::gtest_main
    GTest::gmock_main
    ${PROJECT_NAME}Lib
)

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME}Tests)
