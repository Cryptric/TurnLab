//
// Unit tests for Line class
//

#include <gtest/gtest.h>
#include "Line.h"
#include "Point.h"
#include "Vector.h"
#include <memory>
#include <cmath>

class LineTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code here
    }
    
    void TearDown() override {
        // Cleanup code here
    }
};

// Test default constructor
TEST_F(LineTest, DefaultConstructor) {
    Line line;
    EXPECT_DOUBLE_EQ(line.p1.x, 0.0);
    EXPECT_DOUBLE_EQ(line.p1.y, 0.0);
    EXPECT_DOUBLE_EQ(line.p2.x, 0.0);
    EXPECT_DOUBLE_EQ(line.p2.y, 0.0);
}

// Test constructor with Point objects
TEST_F(LineTest, PointConstructor) {
    Point start(1.0, 2.0);
    Point end(3.0, 4.0);
    Line line(start, end);
    
    EXPECT_DOUBLE_EQ(line.p1.x, 1.0);
    EXPECT_DOUBLE_EQ(line.p1.y, 2.0);
    EXPECT_DOUBLE_EQ(line.p2.x, 3.0);
    EXPECT_DOUBLE_EQ(line.p2.y, 4.0);
}

// Test constructor with coordinate values
TEST_F(LineTest, CoordinateConstructor) {
    Line line(1.5, 2.5, 3.5, 4.5);
    
    EXPECT_DOUBLE_EQ(line.p1.x, 1.5);
    EXPECT_DOUBLE_EQ(line.p1.y, 2.5);
    EXPECT_DOUBLE_EQ(line.p2.x, 3.5);
    EXPECT_DOUBLE_EQ(line.p2.y, 4.5);
}

// Test constructor with negative coordinates
TEST_F(LineTest, NegativeCoordinates) {
    Line line(-2.0, -3.0, 4.0, -1.0);
    
    EXPECT_DOUBLE_EQ(line.p1.x, -2.0);
    EXPECT_DOUBLE_EQ(line.p1.y, -3.0);
    EXPECT_DOUBLE_EQ(line.p2.x, 4.0);
    EXPECT_DOUBLE_EQ(line.p2.y, -1.0);
}

// Test constructor with same start and end points (degenerate line)
TEST_F(LineTest, DegenerateLine) {
    Line line(2.0, 3.0, 2.0, 3.0);
    
    EXPECT_DOUBLE_EQ(line.p1.x, 2.0);
    EXPECT_DOUBLE_EQ(line.p1.y, 3.0);
    EXPECT_DOUBLE_EQ(line.p2.x, 2.0);
    EXPECT_DOUBLE_EQ(line.p2.y, 3.0);
}

// Test equality operator - same lines
TEST_F(LineTest, EqualityOperatorSame) {
    Line line1(1.0, 2.0, 3.0, 4.0);
    Line line2(1.0, 2.0, 3.0, 4.0);
    
    EXPECT_TRUE(line1 == line2);
}

// Test equality operator - reversed lines (should be equal)
TEST_F(LineTest, EqualityOperatorReversed) {
    Line line1(1.0, 2.0, 3.0, 4.0);
    Line line2(3.0, 4.0, 1.0, 2.0);
    
    EXPECT_TRUE(line1 == line2);
}

// Test equality operator - different lines
TEST_F(LineTest, EqualityOperatorDifferent) {
    Line line1(1.0, 2.0, 3.0, 4.0);
    Line line2(1.0, 2.0, 3.0, 5.0);
    
    EXPECT_FALSE(line1 == line2);
}

// Test equality operator - completely different lines
TEST_F(LineTest, EqualityOperatorCompletelyDifferent) {
    Line line1(0.0, 0.0, 1.0, 1.0);
    Line line2(2.0, 2.0, 3.0, 3.0);
    
    EXPECT_FALSE(line1 == line2);
}

// Test equality with degenerate lines
TEST_F(LineTest, EqualityDegenerateLines) {
    Line line1(2.0, 3.0, 2.0, 3.0);
    Line line2(2.0, 3.0, 2.0, 3.0);
    
    EXPECT_TRUE(line1 == line2);
}

// Test clone method
TEST_F(LineTest, CloneMethod) {
    Line original(1.0, 2.0, 3.0, 4.0);
    std::unique_ptr<Segment> cloned = original.clone();
    
    // Cast back to Line to access members
    Line* clonedLine = dynamic_cast<Line*>(cloned.get());
    ASSERT_NE(clonedLine, nullptr);
    
    EXPECT_DOUBLE_EQ(clonedLine->p1.x, 1.0);
    EXPECT_DOUBLE_EQ(clonedLine->p1.y, 2.0);
    EXPECT_DOUBLE_EQ(clonedLine->p2.x, 3.0);
    EXPECT_DOUBLE_EQ(clonedLine->p2.y, 4.0);
    
    // Verify it's a different object
    EXPECT_NE(&original, clonedLine);
}

// Test clone preserves equality
TEST_F(LineTest, ClonePreservesEquality) {
    Line original(2.5, 3.5, 4.5, 5.5);
    std::unique_ptr<Segment> cloned = original.clone();
    
    EXPECT_TRUE(original == *cloned);
}

// Test distance to point on the line
TEST_F(LineTest, DistancePointOnLine) {
    Line line(0.0, 0.0, 4.0, 0.0); // Horizontal line
    Point pointOnLine(2.0, 0.0);
    
    double distance = line.distance(pointOnLine);
    EXPECT_NEAR(distance, 0.0, 1e-10);
}

// Test distance to point perpendicular to line
TEST_F(LineTest, DistancePerpendicularPoint) {
    Line line(0.0, 0.0, 4.0, 0.0); // Horizontal line from (0,0) to (4,0)
    Point point(2.0, 3.0); // Point directly above middle of line
    
    double distance = line.distance(point);
    EXPECT_NEAR(distance, 3.0, 1e-10);
}

// Test distance to point beyond line segment (closer to p1)
TEST_F(LineTest, DistanceBeyondP1) {
    Line line(1.0, 0.0, 3.0, 0.0); // Horizontal line from (1,0) to (3,0)
    Point point(-1.0, 0.0); // Point to the left of p1
    
    double distance = line.distance(point);
    EXPECT_NEAR(distance, 2.0, 1e-10); // Distance from (-1,0) to (1,0)
}

// Test distance to point beyond line segment (closer to p2)
TEST_F(LineTest, DistanceBeyondP2) {
    Line line(1.0, 0.0, 3.0, 0.0); // Horizontal line from (1,0) to (3,0)
    Point point(5.0, 0.0); // Point to the right of p2
    
    double distance = line.distance(point);
    EXPECT_NEAR(distance, 2.0, 1e-10); // Distance from (5,0) to (3,0)
}

// Test distance with vertical line
TEST_F(LineTest, DistanceVerticalLine) {
    Line line(0.0, 0.0, 0.0, 4.0); // Vertical line from (0,0) to (0,4)
    Point point(3.0, 2.0); // Point to the right of middle of line
    
    double distance = line.distance(point);
    EXPECT_NEAR(distance, 3.0, 1e-10);
}

// Test distance with diagonal line (3-4-5 triangle)
TEST_F(LineTest, DistanceDiagonalLine) {
    Line line(0.0, 0.0, 3.0, 4.0); // Line from origin to (3,4)
    Point point(4.0, -3.0); // Point that should be distance 5 from origin
    
    double distance = line.distance(point);
    // This should be the perpendicular distance to the line
    EXPECT_NEAR(distance, 5.0, 1e-10);
}

// Test distance with degenerate line (point)
TEST_F(LineTest, DistanceDegenerateLine) {
    Line line(2.0, 3.0, 2.0, 3.0); // Degenerate line (single point)
    Point point(5.0, 7.0);
    
    double distance = line.distance(point);
    double expectedDistance = std::sqrt((5.0-2.0)*(5.0-2.0) + (7.0-3.0)*(7.0-3.0));
    EXPECT_NEAR(distance, expectedDistance, 1e-10);
}

// Test distance with negative coordinates
TEST_F(LineTest, DistanceNegativeCoordinates) {
    Line line(-2.0, -1.0, 2.0, 3.0); // Line from (-2,-1) to (2,3)
    Point point(0.0, 1.0); // Point on the line
    
    double distance = line.distance(point);
    EXPECT_NEAR(distance, 0.0, 1e-10);
}

// Test distance calculation precision
TEST_F(LineTest, DistancePrecision) {
    Line line(0.0, 0.0, 1.0, 1.0); // 45-degree line
    Point point(0.0, 1.0); // Point forming right triangle
    
    double distance = line.distance(point);
    double expectedDistance = std::sqrt(2.0) / 2.0; // Distance to 45-degree line
    EXPECT_NEAR(distance, expectedDistance, 1e-10);
}

// Test horizontal line edge cases
TEST_F(LineTest, HorizontalLineEdgeCases) {
    Line line(-3.0, 5.0, 7.0, 5.0); // Horizontal line at y=5
    
    // Point directly above
    Point above(2.0, 8.0);
    EXPECT_NEAR(line.distance(above), 3.0, 1e-10);
    
    // Point directly below
    Point below(2.0, 2.0);
    EXPECT_NEAR(line.distance(below), 3.0, 1e-10);
    
    // Point at same y-level but beyond segment
    Point beyond(10.0, 5.0);
    EXPECT_NEAR(line.distance(beyond), 3.0, 1e-10); // Distance to (7,5)
}

// Test vertical line edge cases
TEST_F(LineTest, VerticalLineEdgeCases) {
    Line line(4.0, -2.0, 4.0, 6.0); // Vertical line at x=4
    
    // Point directly to the right
    Point right(7.0, 2.0);
    EXPECT_NEAR(line.distance(right), 3.0, 1e-10);
    
    // Point directly to the left
    Point left(1.0, 2.0);
    EXPECT_NEAR(line.distance(left), 3.0, 1e-10);
    
    // Point at same x-level but beyond segment
    Point beyond(4.0, 10.0);
    EXPECT_NEAR(line.distance(beyond), 4.0, 1e-10); // Distance to (4,6)
}

// Test copy construction
TEST_F(LineTest, CopyConstruction) {
    Line original(1.0, 2.0, 3.0, 4.0);
    Line copy(original);
    
    EXPECT_DOUBLE_EQ(copy.p1.x, 1.0);
    EXPECT_DOUBLE_EQ(copy.p1.y, 2.0);
    EXPECT_DOUBLE_EQ(copy.p2.x, 3.0);
    EXPECT_DOUBLE_EQ(copy.p2.y, 4.0);
    
    // Verify they are equal but different objects
    EXPECT_TRUE(original == copy);
    EXPECT_NE(&original, &copy);
}

// Test assignment operator
TEST_F(LineTest, AssignmentOperator) {
    Line line1(1.0, 2.0, 3.0, 4.0);
    Line line2;
    
    line2 = line1;
    
    EXPECT_DOUBLE_EQ(line2.p1.x, 1.0);
    EXPECT_DOUBLE_EQ(line2.p1.y, 2.0);
    EXPECT_DOUBLE_EQ(line2.p2.x, 3.0);
    EXPECT_DOUBLE_EQ(line2.p2.y, 4.0);
    
    EXPECT_TRUE(line1 == line2);
}

// Test modification of points
TEST_F(LineTest, PointModification) {
    Line line(1.0, 2.0, 3.0, 4.0);
    
    line.p1.x = 10.0;
    line.p1.y = 20.0;
    line.p2.x = 30.0;
    line.p2.y = 40.0;
    
    EXPECT_DOUBLE_EQ(line.p1.x, 10.0);
    EXPECT_DOUBLE_EQ(line.p1.y, 20.0);
    EXPECT_DOUBLE_EQ(line.p2.x, 30.0);
    EXPECT_DOUBLE_EQ(line.p2.y, 40.0);
}

// Test extreme coordinate values
TEST_F(LineTest, ExtremeValues) {
    Line line(1e10, -1e10, -1e10, 1e10);
    
    EXPECT_DOUBLE_EQ(line.p1.x, 1e10);
    EXPECT_DOUBLE_EQ(line.p1.y, -1e10);
    EXPECT_DOUBLE_EQ(line.p2.x, -1e10);
    EXPECT_DOUBLE_EQ(line.p2.y, 1e10);
    
    // Test that equality still works with extreme values
    Line line2(1e10, -1e10, -1e10, 1e10);
    EXPECT_TRUE(line == line2);
}

// Test floating point precision in construction
TEST_F(LineTest, FloatingPointPrecision) {
    double x = 0.1 + 0.2; // May have precision issues
    Line line(x, 1.0/3.0, 0.5, 2.0/3.0);
    
    EXPECT_NEAR(line.p1.x, 0.3, 1e-15);
    EXPECT_NEAR(line.p1.y, 1.0/3.0, 1e-15);
    EXPECT_NEAR(line.p2.x, 0.5, 1e-15);
    EXPECT_NEAR(line.p2.y, 2.0/3.0, 1e-15);
}

// Test polymorphic behavior through Segment interface
TEST_F(LineTest, PolymorphicBehavior) {
    std::unique_ptr<Segment> segment = std::make_unique<Line>(0.0, 0.0, 3.0, 4.0);
    Point testPoint(0.0, 5.0);
    
    double distance = segment->distance(testPoint);
    EXPECT_NEAR(distance, 3.0, 1e-10);
    
    // Test cloning through polymorphic interface
    std::unique_ptr<Segment> cloned = segment->clone();
    EXPECT_TRUE(*segment == *cloned);
}

// Test equality with different segment types
TEST_F(LineTest, EqualityWithDifferentSegmentTypes) {
    Line line(1.0, 2.0, 3.0, 4.0);
    
    // Create a mock segment that's not a Line
    class MockSegment : public Segment {
    public:
        double distance(const Point& point) const override { return 0.0; }
        bool operator==(const Segment& other) const override { return false; }
        std::unique_ptr<Segment> clone() const override { return std::make_unique<MockSegment>(); }
    };
    
    MockSegment mockSegment;
    // Use explicit casting to avoid C++20 ambiguity
    EXPECT_FALSE(static_cast<const Segment&>(line) == static_cast<const Segment&>(mockSegment));
}

// Test distance calculation edge case: point exactly at line endpoints
TEST_F(LineTest, DistanceAtEndpoints) {
    Line line(1.0, 1.0, 4.0, 5.0);
    
    Point atP1(1.0, 1.0);
    Point atP2(4.0, 5.0);
    
    EXPECT_NEAR(line.distance(atP1), 0.0, 1e-10);
    EXPECT_NEAR(line.distance(atP2), 0.0, 1e-10);
}

// Test distance with very small line segment
TEST_F(LineTest, DistanceSmallSegment) {
    Line line(0.0, 0.0, 1e-6, 1e-6);
    Point point(1e-3, 0.0);
    
    double distance = line.distance(point);
    // Should be approximately the distance to the closer endpoint
    EXPECT_GT(distance, 0.0);
    EXPECT_LT(distance, 1e-2);
}

// Test robustness with zero-length vectors in distance calculation
TEST_F(LineTest, DistanceZeroLengthHandling) {
    Line line(5.0, 5.0, 5.0, 5.0); // Zero-length line
    Point point(8.0, 9.0);
    
    double distance = line.distance(point);
    double expectedDistance = std::sqrt((8.0-5.0)*(8.0-5.0) + (9.0-5.0)*(9.0-5.0));
    EXPECT_NEAR(distance, expectedDistance, 1e-10);
}