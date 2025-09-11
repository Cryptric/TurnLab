# Test configuration for TurnLab

# Enable testing
enable_testing()

# Find Google Test
find_package(GTest REQUIRED)
include(GoogleTest)

# Create test executable
add_executable(TurnLabTests
        PointTest.cpp
        DXFUtilsTest.cpp
)

target_link_libraries(TurnLabTests
        TurnLabCore
        GTest::gtest_main
        GTest::gtest
)

# Define source directory macro for tests
target_compile_definitions(TurnLabTests PRIVATE
        TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/test/data"
)

# Discover tests
gtest_discover_tests(TurnLabTests)