#pragma once

#include "Geometry/shapes.h"
#include "LinearAlgebra/lin_alg.h"

#ifdef __cplusplus
extern "C" {
#endif

void drawShapeArray(Shape* shapes, int shapeCount, GLuint shaderProgram);

void drawCircle(Circle circle, GLuint shaderProgram);

void drawRect(Rect rect, GLuint shaderProgram);

void drawCube(Cube cube, GLuint shaderProgram);

#ifdef __cplusplus
}
#endif