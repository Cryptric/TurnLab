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
        ../src/model/geometry/Point.cpp
        ../src/model/geometry/Line.cpp
        ../src/utils/DXFUtils.cpp
        ../src/model/geometry/Geometry.cpp
)

target_include_directories(TurnLabTests PRIVATE
        ../src/model/geometry
        ../src/utils
)

target_link_libraries(TurnLabTests
        GTest::gtest_main
        GTest::gtest
        spdlog::spdlog
        dime::dime
)

# Define source directory macro for tests
target_compile_definitions(TurnLabTests PRIVATE
        TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/test/data"
)

# Discover tests
gtest_discover_tests(TurnLabTests)