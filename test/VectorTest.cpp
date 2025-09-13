//
// Unit tests for Vector class
//

#include <gtest/gtest.h>
#include "Vector.h"
#include <vector>
#include <cmath>

class VectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code here
    }
    
    void TearDown() override {
        // Cleanup code here
    }
};

// Test default constructor
TEST_F(VectorTest, DefaultConstructor) {
    Vector v;
    EXPECT_DOUBLE_EQ(v.x, 0.0);
    EXPECT_DOUBLE_EQ(v.y, 0.0);
}

// Test parameterized constructor
TEST_F(VectorTest, ParameterizedConstructor) {
    Vector v(3.5, -2.1);
    EXPECT_DOUBLE_EQ(v.x, 3.5);
    EXPECT_DOUBLE_EQ(v.y, -2.1);
}

// Test vector constructor - valid vector with exactly 2 elements
TEST_F(VectorTest, VectorConstructorValid) {
    std::vector<double> vec = {1.5, 2.5};
    Vector v(vec);
    EXPECT_DOUBLE_EQ(v.x, 1.5);
    EXPECT_DOUBLE_EQ(v.y, 2.5);
}

// Test vector constructor - with zero values
TEST_F(VectorTest, VectorConstructorZero) {
    std::vector<double> vec = {0.0, 0.0};
    Vector v(vec);
    EXPECT_DOUBLE_EQ(v.x, 0.0);
    EXPECT_DOUBLE_EQ(v.y, 0.0);
}

// Test vector constructor - insufficient elements (should throw)
TEST_F(VectorTest, VectorConstructorInsufficient) {
    std::vector<double> vec = {1.0}; // Only one element
    EXPECT_THROW(Vector v(vec), std::invalid_argument);
}

// Test vector constructor - empty vector (should throw)
TEST_F(VectorTest, VectorConstructorEmpty) {
    std::vector<double> vec;
    EXPECT_THROW(Vector v(vec), std::invalid_argument);
}

// Test vector constructor - too many elements (should throw)
TEST_F(VectorTest, VectorConstructorTooMany) {
    std::vector<double> vec = {1.0, 2.0, 3.0}; // Three elements
    EXPECT_THROW(Vector v(vec), std::invalid_argument);
}

// Test assignment from vector - valid with exactly 2 elements
TEST_F(VectorTest, AssignmentFromVectorValid) {
    Vector v(1.0, 2.0);
    std::vector<double> vec = {10.0, 20.0};
    v = vec;
    EXPECT_DOUBLE_EQ(v.x, 10.0);
    EXPECT_DOUBLE_EQ(v.y, 20.0);
}

// Test assignment from vector - with negative values
TEST_F(VectorTest, AssignmentFromVectorNegative) {
    Vector v(1.0, 2.0);
    std::vector<double> vec = {-7.0, -8.0};
    v = vec;
    EXPECT_DOUBLE_EQ(v.x, -7.0);
    EXPECT_DOUBLE_EQ(v.y, -8.0);
}

// Test assignment from vector - insufficient elements (should throw)
TEST_F(VectorTest, AssignmentFromVectorInsufficient) {
    Vector v(1.0, 2.0);
    std::vector<double> vec = {5.0}; // Only one element
    EXPECT_THROW(v = vec, std::invalid_argument);
    // Ensure original values unchanged after exception
    EXPECT_DOUBLE_EQ(v.x, 1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
}

// Test assignment from vector - empty (should throw)
TEST_F(VectorTest, AssignmentFromVectorEmpty) {
    Vector v(1.0, 2.0);
    std::vector<double> vec;
    EXPECT_THROW(v = vec, std::invalid_argument);
    // Ensure original values unchanged after exception
    EXPECT_DOUBLE_EQ(v.x, 1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
}

// Test assignment from vector - too many elements (should throw)
TEST_F(VectorTest, AssignmentFromVectorTooMany) {
    Vector v(1.0, 2.0);
    std::vector<double> vec = {5.0, 6.0, 7.0}; // Three elements
    EXPECT_THROW(v = vec, std::invalid_argument);
    // Ensure original values unchanged after exception
    EXPECT_DOUBLE_EQ(v.x, 1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
}

// Test addition operator
TEST_F(VectorTest, AdditionOperator) {
    Vector v1(2.0, 3.0);
    Vector v2(4.0, 5.0);
    Vector result = v1 + v2;
    EXPECT_DOUBLE_EQ(result.x, 6.0);
    EXPECT_DOUBLE_EQ(result.y, 8.0);
    
    // Ensure original vectors unchanged
    EXPECT_DOUBLE_EQ(v1.x, 2.0);
    EXPECT_DOUBLE_EQ(v1.y, 3.0);
    EXPECT_DOUBLE_EQ(v2.x, 4.0);
    EXPECT_DOUBLE_EQ(v2.y, 5.0);
}

// Test addition with zero vector
TEST_F(VectorTest, AdditionWithZero) {
    Vector v1(3.0, 4.0);
    Vector zero(0.0, 0.0);
    Vector result = v1 + zero;
    EXPECT_DOUBLE_EQ(result.x, 3.0);
    EXPECT_DOUBLE_EQ(result.y, 4.0);
}

// Test addition with negative values
TEST_F(VectorTest, AdditionWithNegative) {
    Vector v1(5.0, -2.0);
    Vector v2(-3.0, 7.0);
    Vector result = v1 + v2;
    EXPECT_DOUBLE_EQ(result.x, 2.0);
    EXPECT_DOUBLE_EQ(result.y, 5.0);
}

// Test subtraction operator
TEST_F(VectorTest, SubtractionOperator) {
    Vector v1(7.0, 9.0);
    Vector v2(3.0, 4.0);
    Vector result = v1 - v2;
    EXPECT_DOUBLE_EQ(result.x, 4.0);
    EXPECT_DOUBLE_EQ(result.y, 5.0);
    
    // Ensure original vectors unchanged
    EXPECT_DOUBLE_EQ(v1.x, 7.0);
    EXPECT_DOUBLE_EQ(v1.y, 9.0);
    EXPECT_DOUBLE_EQ(v2.x, 3.0);
    EXPECT_DOUBLE_EQ(v2.y, 4.0);
}

// Test subtraction with zero vector
TEST_F(VectorTest, SubtractionWithZero) {
    Vector v1(5.0, 6.0);
    Vector zero(0.0, 0.0);
    Vector result = v1 - zero;
    EXPECT_DOUBLE_EQ(result.x, 5.0);
    EXPECT_DOUBLE_EQ(result.y, 6.0);
}

// Test subtraction with negative values
TEST_F(VectorTest, SubtractionWithNegative) {
    Vector v1(2.0, 3.0);
    Vector v2(-4.0, -5.0);
    Vector result = v1 - v2;
    EXPECT_DOUBLE_EQ(result.x, 6.0);
    EXPECT_DOUBLE_EQ(result.y, 8.0);
}

// Test dot product
TEST_F(VectorTest, DotProduct) {
    Vector v1(3.0, 4.0);
    Vector v2(2.0, 5.0);
    double result = v1.dot(v2);
    EXPECT_DOUBLE_EQ(result, 26.0); // 3*2 + 4*5 = 6 + 20 = 26
}

// Test dot product with zero vector
TEST_F(VectorTest, DotProductWithZero) {
    Vector v1(5.0, 3.0);
    Vector zero(0.0, 0.0);
    double result = v1.dot(zero);
    EXPECT_DOUBLE_EQ(result, 0.0);
}

// Test dot product with same vector (should give norm squared)
TEST_F(VectorTest, DotProductWithSelf) {
    Vector v(3.0, 4.0);
    double result = v.dot(v);
    EXPECT_DOUBLE_EQ(result, 25.0); // 3*3 + 4*4 = 9 + 16 = 25
}

// Test dot product with negative values
TEST_F(VectorTest, DotProductNegative) {
    Vector v1(-2.0, 3.0);
    Vector v2(4.0, -1.0);
    double result = v1.dot(v2);
    EXPECT_DOUBLE_EQ(result, -11.0); // -2*4 + 3*(-1) = -8 + (-3) = -11
}

// Test dot product perpendicular vectors
TEST_F(VectorTest, DotProductPerpendicular) {
    Vector v1(1.0, 0.0);
    Vector v2(0.0, 1.0);
    double result = v1.dot(v2);
    EXPECT_DOUBLE_EQ(result, 0.0);
}

// Test norm (magnitude)
TEST_F(VectorTest, Norm) {
    Vector v(3.0, 4.0);
    double result = v.norm();
    EXPECT_DOUBLE_EQ(result, 5.0); // sqrt(3^2 + 4^2) = sqrt(25) = 5
}

// Test norm of zero vector
TEST_F(VectorTest, NormZero) {
    Vector v(0.0, 0.0);
    double result = v.norm();
    EXPECT_DOUBLE_EQ(result, 0.0);
}

// Test norm of unit vectors
TEST_F(VectorTest, NormUnit) {
    Vector v1(1.0, 0.0);
    Vector v2(0.0, 1.0);
    EXPECT_DOUBLE_EQ(v1.norm(), 1.0);
    EXPECT_DOUBLE_EQ(v2.norm(), 1.0);
}

// Test norm with negative values
TEST_F(VectorTest, NormNegative) {
    Vector v(-3.0, -4.0);
    double result = v.norm();
    EXPECT_DOUBLE_EQ(result, 5.0); // sqrt((-3)^2 + (-4)^2) = sqrt(25) = 5
}

// Test norm with decimal values
TEST_F(VectorTest, NormDecimal) {
    Vector v(1.0, 1.0);
    double result = v.norm();
    EXPECT_NEAR(result, std::sqrt(2.0), 1e-15);
}

// Test equality operator
TEST_F(VectorTest, EqualityOperator) {
    Vector v1(3.0, 4.0);
    Vector v2(3.0, 4.0);
    Vector v3(3.0, 5.0);
    
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
}

// Test equality with zero vectors
TEST_F(VectorTest, EqualityZero) {
    Vector v1(0.0, 0.0);
    Vector v2(0.0, 0.0);
    EXPECT_TRUE(v1 == v2);
}

// Test equality with negative values
TEST_F(VectorTest, EqualityNegative) {
    Vector v1(-2.5, -3.5);
    Vector v2(-2.5, -3.5);
    Vector v3(-2.5, 3.5);
    
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
}

// Test inequality operator
TEST_F(VectorTest, InequalityOperator) {
    Vector v1(3.0, 4.0);
    Vector v2(3.0, 4.0);
    Vector v3(3.0, 5.0);
    Vector v4(4.0, 4.0);
    
    EXPECT_FALSE(v1 != v2);
    EXPECT_TRUE(v1 != v3);
    EXPECT_TRUE(v1 != v4);
}

// Test indexing operator - valid indices
TEST_F(VectorTest, IndexingOperatorValid) {
    Vector v(3.0, 4.0);
    EXPECT_DOUBLE_EQ(v[0], 3.0);
    EXPECT_DOUBLE_EQ(v[1], 4.0);
}

// Test indexing operator - modification
TEST_F(VectorTest, IndexingOperatorModification) {
    Vector v(1.0, 2.0);
    v[0] = 10.0;
    v[1] = 20.0;
    EXPECT_DOUBLE_EQ(v.x, 10.0);
    EXPECT_DOUBLE_EQ(v.y, 20.0);
}

// Test const indexing operator
TEST_F(VectorTest, ConstIndexingOperator) {
    const Vector v(5.0, 6.0);
    EXPECT_DOUBLE_EQ(v[0], 5.0);
    EXPECT_DOUBLE_EQ(v[1], 6.0);
}

// Test indexing operator - invalid indices
TEST_F(VectorTest, IndexingOperatorInvalid) {
    Vector v(1.0, 2.0);
    EXPECT_THROW(v[2], std::out_of_range);
    EXPECT_THROW(v[-1], std::out_of_range);
    EXPECT_THROW(v[100], std::out_of_range);
}

// Test const indexing operator - invalid indices
TEST_F(VectorTest, ConstIndexingOperatorInvalid) {
    const Vector v(1.0, 2.0);
    EXPECT_THROW(v[2], std::out_of_range);
    EXPECT_THROW(v[-1], std::out_of_range);
}

// Test chaining operations
TEST_F(VectorTest, ChainingOperations) {
    Vector v1(1.0, 2.0);
    Vector v2(3.0, 4.0);
    Vector v3(5.0, 6.0);
    
    Vector result = v1 + v2 - v3;
    EXPECT_DOUBLE_EQ(result.x, -1.0); // 1 + 3 - 5
    EXPECT_DOUBLE_EQ(result.y, 0.0);  // 2 + 4 - 6
}

// Test extreme values
TEST_F(VectorTest, ExtremeValues) {
    Vector v(1e10, -1e10);
    EXPECT_DOUBLE_EQ(v.x, 1e10);
    EXPECT_DOUBLE_EQ(v.y, -1e10);
    
    Vector v2(1e-10, -1e-10);
    EXPECT_DOUBLE_EQ(v2.x, 1e-10);
    EXPECT_DOUBLE_EQ(v2.y, -1e-10);
}

// Test floating point precision
TEST_F(VectorTest, FloatingPointPrecision) {
    Vector v(0.1 + 0.2, 1.0/3.0);
    EXPECT_NEAR(v.x, 0.3, 1e-15);
    EXPECT_NEAR(v.y, 1.0/3.0, 1e-15);
}

// Test norm with very large values
TEST_F(VectorTest, NormLargeValues) {
    Vector v(1e6, 1e6);
    double result = v.norm();
    EXPECT_NEAR(result, std::sqrt(2e12), 1e6);
}

// Test norm with very small values
TEST_F(VectorTest, NormSmallValues) {
    Vector v(1e-6, 1e-6);
    double result = v.norm();
    EXPECT_NEAR(result, std::sqrt(2e-12), 1e-15);
}

// Test dot product commutativity
TEST_F(VectorTest, DotProductCommutativity) {
    Vector v1(2.5, 3.7);
    Vector v2(-1.2, 4.8);
    double result1 = v1.dot(v2);
    double result2 = v2.dot(v1);
    EXPECT_DOUBLE_EQ(result1, result2);
}

// Test self-assignment edge case
TEST_F(VectorTest, SelfAssignmentVector) {
    Vector v(3.0, 4.0);
    std::vector<double> vec = {3.0, 4.0};
    v = vec;
    EXPECT_DOUBLE_EQ(v.x, 3.0);
    EXPECT_DOUBLE_EQ(v.y, 4.0);
}

// Test assignment return value
TEST_F(VectorTest, AssignmentReturnValue) {
    Vector v1(1.0, 2.0);
    Vector v2(3.0, 4.0);
    std::vector<double> vec = {5.0, 6.0};
    
    Vector& result = (v1 = vec);
    EXPECT_EQ(&result, &v1); // Should return reference to v1
    EXPECT_DOUBLE_EQ(v1.x, 5.0);
    EXPECT_DOUBLE_EQ(v1.y, 6.0);
}

// Test copy construction behavior
TEST_F(VectorTest, CopyConstruction) {
    Vector v1(3.5, 4.5);
    Vector v2(v1);
    EXPECT_DOUBLE_EQ(v2.x, 3.5);
    EXPECT_DOUBLE_EQ(v2.y, 4.5);
    
    // Modify original, copy should be unchanged
    v1.x = 10.0;
    EXPECT_DOUBLE_EQ(v2.x, 3.5);
}

// Test mathematical properties - distributivity of dot product
TEST_F(VectorTest, DotProductDistributivity) {
    Vector v1(2.0, 3.0);
    Vector v2(4.0, 1.0);
    Vector v3(1.0, 2.0);
    
    double left = v1.dot(v2 + v3);
    double right = v1.dot(v2) + v1.dot(v3);
    EXPECT_NEAR(left, right, 1e-15);
}

// Test mathematical properties - addition associativity
TEST_F(VectorTest, AdditionAssociativity) {
    Vector v1(1.0, 2.0);
    Vector v2(3.0, 4.0);
    Vector v3(5.0, 6.0);
    
    Vector left = (v1 + v2) + v3;
    Vector right = v1 + (v2 + v3);
    EXPECT_DOUBLE_EQ(left.x, right.x);
    EXPECT_DOUBLE_EQ(left.y, right.y);
}

// Test mathematical properties - addition commutativity
TEST_F(VectorTest, AdditionCommutativity) {
    Vector v1(2.5, 3.7);
    Vector v2(-1.2, 4.8);
    
    Vector left = v1 + v2;
    Vector right = v2 + v1;
    EXPECT_DOUBLE_EQ(left.x, right.x);
    EXPECT_DOUBLE_EQ(left.y, right.y);
}