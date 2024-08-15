#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Common/types.h"

#define PI 3.141592654f

#define DIMENTION_COUNT 3
#define COLOR_COUNT 3

typedef struct {
    float x;
    float y;
    float z;
} Point;

typedef struct {
    Point* vertices;
    int verticesCount;
    GLenum drawingMethod;
    Color color;
} Shape;

typedef struct {
    Point centerPoint;
    float radius;
    Color color;
} Circle;

typedef struct {
    Point vertices[8];
} Cube;

typedef struct {
    Point vertices[4];
    Color color;
} Rect;

void mapShapeToGLVertices(Shape shape, GLfloat* vertices, int dim);

void rotatePoint(Point* point, float angleDegs, Point pointOfRotation);

void rotateShape(Shape* shape, float angleDegs, Point pointOfRotation);

Point getShapeCenter(Shape shape);

float degsToRads(float angleDegs);

#ifdef __cplusplus
}
#endif