#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "shapes.h"

using namespace testing;

struct GeometryShould : public Test {
    void SetUp() {}
};

TEST_F(GeometryShould, rotatePointProperly) {
    Point point1 = {1.0f, 1.0f};
    Point point2 = point1;
    Point pointOfRotation = {0.0f, 0.0f};

    rotatePoint(&point1, 10.0f, pointOfRotation);
    rotatePoint(&point2, 370.0f, pointOfRotation);

    EXPECT_NEAR(point1.x, point2.x, .000001);
    EXPECT_NEAR(point1.y, point2.y, .000001);
}

TEST_F(GeometryShould, rotateShapeProperly) {
    Point triangle1Vertices[] = {{1.0f, 2.0f}, {3.0f, 4.0f}, {5.0f, 6.0f}};
    Point triangle2Vertices[] = {{1.0f, 2.0f}, {3.0f, 4.0f}, {5.0f, 6.0f}};
    Shape triangle1 = {triangle1Vertices, 3, GL_TRIANGLES};
    Shape triangle2 = {triangle2Vertices, 3, GL_TRIANGLES};
    Point rotationPoint = {1.0f, 1.0f};
    rotateShape(&triangle1, 10.0f, rotationPoint);
    rotateShape(&triangle2, 370.0f, rotationPoint);
    for (int i = 0; i < triangle1.verticesCount; i++) {
        EXPECT_NEAR(triangle1.vertices[i].x, triangle2.vertices[i].x, 0.000001);
        EXPECT_NEAR(triangle1.vertices[i].y, triangle2.vertices[i].y, 0.000001);
    }
}

TEST_F(GeometryShould, convertShapeToVerticesAllDimenstion) {
    Point shapeVertices[3][3] = {{{0.0f}, {1.0f}, {2.0f}},
                                 {{0.0f, 0.0f}, {1.0f, 1.0f}, {2.0f, 0.0f}},
                                 {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {2.0f, 0.0f, 2.0f}}};
    Shape shapes[3] = {{shapeVertices[0], 3, GL_TRIANGLES, {0.2f, 0.6f, .80f}},
                       {shapeVertices[1], 3, GL_TRIANGLES, {0.1f, 0.4f, .11f}},
                       {shapeVertices[2], 3, GL_TRIANGLES, {1.0f, 2.0f, .30f}}};
    for (int dim = 1; dim <= 3; dim++) {
        GLfloat* shapeVertices = (GLfloat*)malloc(shapes[dim - 1].verticesCount * 6 * sizeof(GLfloat));
        mapShapeToGLVertices(shapes[dim - 1], shapeVertices, dim);
        for (int j = 0; j < shapes[dim - 1].verticesCount; j++) {
            EXPECT_NEAR(shapes[dim - 1].vertices[j].x, shapeVertices[j * 6], 0.000001);
            EXPECT_NEAR(shapes[dim - 1].vertices[j].y, shapeVertices[j * 6 + 1], 0.000001);
            EXPECT_NEAR(shapes[dim - 1].vertices[j].z, shapeVertices[j * 6 + 2], 0.000001);
            EXPECT_NEAR(shapes[dim - 1].color.r, shapeVertices[j * 6 + 3], 0.000001);
            EXPECT_NEAR(shapes[dim - 1].color.g, shapeVertices[j * 6 + 4], 0.000001);
            EXPECT_NEAR(shapes[dim - 1].color.b, shapeVertices[j * 6 + 5], 0.000001);
        }
        free(shapeVertices);
    }
}

TEST_F(GeometryShould, dieIfWrongDimensionInShapeConversion) {
    Point shapeVertices[] = {{0.0f}, {1.0f}, {2.0f}};
    Shape shape = {shapeVertices, 3, GL_TRIANGLES};
    GLfloat* shapeVerticesGL = (GLfloat*)malloc(shape.verticesCount * 3 * sizeof(GLfloat));
    EXPECT_DEATH(mapShapeToGLVertices(shape, shapeVerticesGL, 4), "");
    free(shapeVerticesGL);
}

TEST_F(GeometryShould, findShapeCenterProperly) {
    Point shapeVertices[] = {{0.0f, 0.0f, 14.0f}, {1.0f, 1.0f, 1.0f}, {2.0f, 5.0f, 0.0f}};
    Shape triangle = {shapeVertices, 3, GL_TRIANGLES};
    Point center = getShapeCenter(triangle);
    Point expectedCenter = {1.0f, 2.0f, 5.0f};
    EXPECT_NEAR(center.x, expectedCenter.x, .0000001);
    EXPECT_NEAR(center.y, expectedCenter.y, .0000001);
    EXPECT_NEAR(center.z, expectedCenter.z, .0000001);
}