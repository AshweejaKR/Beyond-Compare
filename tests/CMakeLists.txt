include(FetchContent)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)

add_executable(bcclone_tests
    LoggerTest.cpp
)

target_link_libraries(bcclone_tests PRIVATE
    bcclone_utils
    GTest::gtest_main
)

bcclone_set_warnings(bcclone_tests)

include(GoogleTest)
gtest_discover_tests(bcclone_tests)
