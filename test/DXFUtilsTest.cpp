//
// Unit tests for DXFUtils class
//

#include <algorithm>
#include <gtest/gtest.h>
#include <filesystem>

#include "Line.h"
#include "../src/utils/DXFUtils.h"
#include "../src/model/geometry/Geometry.h"

class DXFUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code here
    }
    
    void TearDown() override {
        // Cleanup code here
    }
};

// Test loading DXF file from test data directory
TEST_F(DXFUtilsTest, LoadDXFFile) {
    std::filesystem::path dxfPath = std::filesystem::path(TEST_DATA_DIR) / "Lines.dxf";
    
    Geometry geometry = loadDXF(dxfPath);

    EXPECT_EQ(geometry.segments.size(), 4);

    std::vector<Line> expectedLines = {
        {{0, 0}, {50, 0}},
        {{50, 0}, {50, 60}},
        {{50, 60}, {0, 10}},
        {{0, 10}, {0, 0}}
    };

    for (size_t i = 0; i < expectedLines.size(); i++) {
        bool found = std::ranges::any_of(geometry.segments, [&](const auto& segment) { return *segment == expectedLines[i];});
        EXPECT_TRUE(found);
    }
}