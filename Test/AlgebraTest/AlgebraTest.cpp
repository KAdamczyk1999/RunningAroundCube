#include <math.h>

#include "LinearAlgebra/lin_alg.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace testing;

struct AlgebraShould : public Test {
    void SetUp() {}
};

TEST_F(AlgebraShould, apply3dOperator) {
    float opValues[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Point point = {10, 11, 12};

    Matrix op;
    op.columnCount = 3;
    op.rowCount = 3;
    op.values = (float**)malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) op.values[i] = opValues[i];
    applyOperatorOn3dPoint(op, &point);
    free(op.values);
    EXPECT_EQ(point.x, 68.0f);
    EXPECT_EQ(point.y, 167.0f);
    EXPECT_EQ(point.z, 266.0f);
}

TEST_F(AlgebraShould, generateXRotationOperator) {
    Matrix matrix;
    float rows[3][3];
    matrix.values = (float**)malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) matrix.values[i] = rows[i];
    matrix.columnCount = 3;
    matrix.rowCount = 3;
    float angleRads = 30.0f * PI / 180.0f;
    generateXRotationOperator(&matrix, 30.0f);
    EXPECT_EQ(matrix.values[0][0], 1.0f);
    EXPECT_EQ(matrix.values[0][1], 0.0f);
    EXPECT_EQ(matrix.values[0][2], 0.0f);
    EXPECT_EQ(matrix.values[1][0], 0.0f);
    EXPECT_EQ(matrix.values[1][1], cos(angleRads));
    EXPECT_EQ(matrix.values[1][2], -sin(angleRads));
    EXPECT_EQ(matrix.values[2][0], 0.0f);
    EXPECT_EQ(matrix.values[2][1], sin(angleRads));
    EXPECT_EQ(matrix.values[2][2], cos(angleRads));
}
TEST_F(AlgebraShould, generateYRotationOperator) {
    Matrix matrix;
    float rows[3][3];
    matrix.values = (float**)malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) matrix.values[i] = rows[i];
    matrix.columnCount = 3;
    matrix.rowCount = 3;
    float angleRads = 30.0f * PI / 180.0f;
    generateYRotationOperator(&matrix, 30.0f);
    EXPECT_EQ(matrix.values[0][0], cos(angleRads));
    EXPECT_EQ(matrix.values[0][1], 0.0f);
    EXPECT_EQ(matrix.values[0][2], sin(angleRads));
    EXPECT_EQ(matrix.values[1][0], 0.0f);
    EXPECT_EQ(matrix.values[1][1], 1.0f);
    EXPECT_EQ(matrix.values[1][2], 0.0f);
    EXPECT_EQ(matrix.values[2][0], -sin(angleRads));
    EXPECT_EQ(matrix.values[2][1], 0.0f);
    EXPECT_EQ(matrix.values[2][2], cos(angleRads));
}
TEST_F(AlgebraShould, generateZRotationOperator) {
    Matrix matrix;
    float rows[3][3];
    matrix.values = (float**)malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) matrix.values[i] = rows[i];
    matrix.columnCount = 3;
    matrix.rowCount = 3;
    float angleRads = 30.0f * PI / 180.0f;
    generateZRotationOperator(&matrix, 30.0f);
    EXPECT_EQ(matrix.values[0][0], cos(angleRads));
    EXPECT_EQ(matrix.values[0][1], -sin(angleRads));
    EXPECT_EQ(matrix.values[0][2], 0.0f);
    EXPECT_EQ(matrix.values[1][0], sin(angleRads));
    EXPECT_EQ(matrix.values[1][1], cos(angleRads));
    EXPECT_EQ(matrix.values[1][2], 0.0f);
    EXPECT_EQ(matrix.values[2][0], 0.0f);
    EXPECT_EQ(matrix.values[2][1], 0.0f);
    EXPECT_EQ(matrix.values[2][2], 1.0f);
}

TEST_F(AlgebraShould, calculateLineAngleProperly) {
    Point lines[2] = {{1, 1, 1}, {3, 4, 5}};
    EXPECT_NEAR(0.201357920790331, _calculateLineAngle(lines), 0.000001);
}

TEST_F(AlgebraShould, findOrthogonalVectorProperly) {
    Point ortVecs[2] = {{.0f, .0f, .0f}, {.0f, .0f, .0f}};
    Point vec = {1.0f, 2.5f, 4.0f};
    _findOthrogonalPoints(ortVecs, vec);
    EXPECT_NEAR(_dotProd(ortVecs[0], vec), 0.0f, .000001f);
    EXPECT_NEAR(_dotProd(ortVecs[1], vec), 0.0f, .000001f);
    EXPECT_NEAR(_dotProd(ortVecs[0], ortVecs[1]), 0.0f, .000001f);
}