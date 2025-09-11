//
// Unit tests for Point class
//

#include <gtest/gtest.h>
#include "Point.h"
#include <vector>

class PointTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code here
    }
    
    void TearDown() override {
        // Cleanup code here
    }
};

// Test default constructor
TEST_F(PointTest, DefaultConstructor) {
    Point p;
    EXPECT_DOUBLE_EQ(p.x, 0.0);
    EXPECT_DOUBLE_EQ(p.y, 0.0);
}

// Test parameterized constructor
TEST_F(PointTest, ParameterizedConstructor) {
    Point p(3.5, -2.1);
    EXPECT_DOUBLE_EQ(p.x, 3.5);
    EXPECT_DOUBLE_EQ(p.y, -2.1);
}

// Test vector constructor - valid vector with exactly 2 elements
TEST_F(PointTest, VectorConstructorValid) {
    std::vector<double> vec = {1.5, 2.5};
    Point p(vec);
    EXPECT_DOUBLE_EQ(p.x, 1.5);
    EXPECT_DOUBLE_EQ(p.y, 2.5);
}

// Test vector constructor - with zero values
TEST_F(PointTest, VectorConstructorZero) {
    std::vector<double> vec = {0.0, 0.0};
    Point p(vec);
    EXPECT_DOUBLE_EQ(p.x, 0.0);
    EXPECT_DOUBLE_EQ(p.y, 0.0);
}

// Test vector constructor - insufficient elements (should throw)
TEST_F(PointTest, VectorConstructorInsufficient) {
    std::vector<double> vec = {1.0}; // Only one element
    EXPECT_THROW(Point p(vec), std::invalid_argument);
}

// Test vector constructor - empty vector (should throw)
TEST_F(PointTest, VectorConstructorEmpty) {
    std::vector<double> vec;
    EXPECT_THROW(Point p(vec), std::invalid_argument);
}

// Test vector constructor - too many elements (should throw)
TEST_F(PointTest, VectorConstructorTooMany) {
    std::vector<double> vec = {1.0, 2.0, 3.0}; // Three elements
    EXPECT_THROW(Point p(vec), std::invalid_argument);
}

// Test indexing operator - valid indices
TEST_F(PointTest, IndexingOperatorValid) {
    Point p(3.0, 4.0);
    EXPECT_DOUBLE_EQ(p[0], 3.0);
    EXPECT_DOUBLE_EQ(p[1], 4.0);
}

// Test indexing operator - modification
TEST_F(PointTest, IndexingOperatorModification) {
    Point p(1.0, 2.0);
    p[0] = 10.0;
    p[1] = 20.0;
    EXPECT_DOUBLE_EQ(p.x, 10.0);
    EXPECT_DOUBLE_EQ(p.y, 20.0);
}

// Test const indexing operator
TEST_F(PointTest, ConstIndexingOperator) {
    const Point p(5.0, 6.0);
    EXPECT_DOUBLE_EQ(p[0], 5.0);
    EXPECT_DOUBLE_EQ(p[1], 6.0);
}

// Test indexing operator - invalid indices
TEST_F(PointTest, IndexingOperatorInvalid) {
    Point p(1.0, 2.0);
    EXPECT_THROW(p[2], std::out_of_range);
    EXPECT_THROW(p[-1], std::out_of_range);
    EXPECT_THROW(p[100], std::out_of_range);
}

// Test const indexing operator - invalid indices
TEST_F(PointTest, ConstIndexingOperatorInvalid) {
    const Point p(1.0, 2.0);
    EXPECT_THROW(p[2], std::out_of_range);
    EXPECT_THROW(p[-1], std::out_of_range);
}

// Test addition operator
TEST_F(PointTest, AdditionOperator) {
    Point p1(2.0, 3.0);
    Point p2(4.0, 5.0);
    Point result = p1 + p2;
    EXPECT_DOUBLE_EQ(result.x, 6.0);
    EXPECT_DOUBLE_EQ(result.y, 8.0);
    
    // Ensure original points unchanged
    EXPECT_DOUBLE_EQ(p1.x, 2.0);
    EXPECT_DOUBLE_EQ(p1.y, 3.0);
    EXPECT_DOUBLE_EQ(p2.x, 4.0);
    EXPECT_DOUBLE_EQ(p2.y, 5.0);
}

// Test addition with zero
TEST_F(PointTest, AdditionWithZero) {
    Point p1(3.0, 4.0);
    Point zero(0.0, 0.0);
    Point result = p1 + zero;
    EXPECT_DOUBLE_EQ(result.x, 3.0);
    EXPECT_DOUBLE_EQ(result.y, 4.0);
}

// Test addition with negative values
TEST_F(PointTest, AdditionWithNegative) {
    Point p1(5.0, -2.0);
    Point p2(-3.0, 7.0);
    Point result = p1 + p2;
    EXPECT_DOUBLE_EQ(result.x, 2.0);
    EXPECT_DOUBLE_EQ(result.y, 5.0);
}

// Test subtraction operator
TEST_F(PointTest, SubtractionOperator) {
    Point p1(7.0, 9.0);
    Point p2(3.0, 4.0);
    Point result = p1 - p2;
    EXPECT_DOUBLE_EQ(result.x, 4.0);
    EXPECT_DOUBLE_EQ(result.y, 5.0);
    
    // Ensure original points unchanged
    EXPECT_DOUBLE_EQ(p1.x, 7.0);
    EXPECT_DOUBLE_EQ(p1.y, 9.0);
    EXPECT_DOUBLE_EQ(p2.x, 3.0);
    EXPECT_DOUBLE_EQ(p2.y, 4.0);
}

// Test subtraction with zero
TEST_F(PointTest, SubtractionWithZero) {
    Point p1(5.0, 6.0);
    Point zero(0.0, 0.0);
    Point result = p1 - zero;
    EXPECT_DOUBLE_EQ(result.x, 5.0);
    EXPECT_DOUBLE_EQ(result.y, 6.0);
}

// Test subtraction with negative values
TEST_F(PointTest, SubtractionWithNegative) {
    Point p1(2.0, 3.0);
    Point p2(-4.0, -5.0);
    Point result = p1 - p2;
    EXPECT_DOUBLE_EQ(result.x, 6.0);
    EXPECT_DOUBLE_EQ(result.y, 8.0);
}

// Test assignment from vector - valid with exactly 2 elements
TEST_F(PointTest, AssignmentFromVectorValid) {
    Point p(1.0, 2.0);
    std::vector<double> vec = {10.0, 20.0};
    p = vec;
    EXPECT_DOUBLE_EQ(p.x, 10.0);
    EXPECT_DOUBLE_EQ(p.y, 20.0);
}

// Test assignment from vector - with negative values
TEST_F(PointTest, AssignmentFromVectorNegative) {
    Point p(1.0, 2.0);
    std::vector<double> vec = {-7.0, -8.0};
    p = vec;
    EXPECT_DOUBLE_EQ(p.x, -7.0);
    EXPECT_DOUBLE_EQ(p.y, -8.0);
}

// Test assignment from vector - insufficient elements (should throw)
TEST_F(PointTest, AssignmentFromVectorInsufficient) {
    Point p(1.0, 2.0);
    std::vector<double> vec = {5.0}; // Only one element
    EXPECT_THROW(p = vec, std::invalid_argument);
    // Ensure original values unchanged after exception
    EXPECT_DOUBLE_EQ(p.x, 1.0);
    EXPECT_DOUBLE_EQ(p.y, 2.0);
}

// Test assignment from vector - empty (should throw)
TEST_F(PointTest, AssignmentFromVectorEmpty) {
    Point p(1.0, 2.0);
    std::vector<double> vec;
    EXPECT_THROW(p = vec, std::invalid_argument);
    // Ensure original values unchanged after exception
    EXPECT_DOUBLE_EQ(p.x, 1.0);
    EXPECT_DOUBLE_EQ(p.y, 2.0);
}

// Test assignment from vector - too many elements (should throw)
TEST_F(PointTest, AssignmentFromVectorTooMany) {
    Point p(1.0, 2.0);
    std::vector<double> vec = {5.0, 6.0, 7.0}; // Three elements
    EXPECT_THROW(p = vec, std::invalid_argument);
    // Ensure original values unchanged after exception
    EXPECT_DOUBLE_EQ(p.x, 1.0);
    EXPECT_DOUBLE_EQ(p.y, 2.0);
}

// Test conversion to vector
TEST_F(PointTest, ConversionToVector) {
    Point p(3.14, 2.71);
    std::vector<double> vec = p;
    ASSERT_EQ(vec.size(), 2);
    EXPECT_DOUBLE_EQ(vec[0], 3.14);
    EXPECT_DOUBLE_EQ(vec[1], 2.71);
}

// Test conversion to vector with zero values
TEST_F(PointTest, ConversionToVectorZero) {
    Point p(0.0, 0.0);
    std::vector<double> vec = p;
    ASSERT_EQ(vec.size(), 2);
    EXPECT_DOUBLE_EQ(vec[0], 0.0);
    EXPECT_DOUBLE_EQ(vec[1], 0.0);
}

// Test conversion to vector with negative values
TEST_F(PointTest, ConversionToVectorNegative) {
    Point p(-1.5, -2.5);
    std::vector<double> vec = p;
    ASSERT_EQ(vec.size(), 2);
    EXPECT_DOUBLE_EQ(vec[0], -1.5);
    EXPECT_DOUBLE_EQ(vec[1], -2.5);
}

// Test chaining operations
TEST_F(PointTest, ChainingOperations) {
    Point p1(1.0, 2.0);
    Point p2(3.0, 4.0);
    Point p3(5.0, 6.0);
    
    Point result = p1 + p2 - p3;
    EXPECT_DOUBLE_EQ(result.x, -1.0); // 1 + 3 - 5
    EXPECT_DOUBLE_EQ(result.y, 0.0);  // 2 + 4 - 6
}

// Test extreme values
TEST_F(PointTest, ExtremeValues) {
    Point p(1e10, -1e10);
    EXPECT_DOUBLE_EQ(p.x, 1e10);
    EXPECT_DOUBLE_EQ(p.y, -1e10);
    
    Point p2(1e-10, -1e-10);
    EXPECT_DOUBLE_EQ(p2.x, 1e-10);
    EXPECT_DOUBLE_EQ(p2.y, -1e-10);
}

// Test floating point precision
TEST_F(PointTest, FloatingPointPrecision) {
    Point p(0.1 + 0.2, 1.0/3.0);
    EXPECT_NEAR(p.x, 0.3, 1e-15);
    EXPECT_NEAR(p.y, 1.0/3.0, 1e-15);
}