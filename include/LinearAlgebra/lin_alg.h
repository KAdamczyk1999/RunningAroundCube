#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "Geometry/shapes.h"

typedef struct {
    float** values;
    int rowCount;
    int columnCount;
} Matrix;

void applyOperatorOn3dPoint(Matrix op, Point* point);
void generateXRotationOperator(Matrix* matrix, float angle);
void generateYRotationOperator(Matrix* matrix, float angle);
void generateZRotationOperator(Matrix* matrix, float angle);
float calculateDist(Point p1, Point p2);
float calculateRectProjectionArea(Rect rect, Point pointOfVIew);
float calculateRectArea(Rect rect);
void projectRect(Rect* rect, Point pointOfView);
void simpleProjectRect(Rect* rect);

#ifdef UT
float _calculateLineAngle(Point* lines);
float _dotProd(Point p1, Point p2);
void _findOthrogonalPoints(Point* points, Point point);
#endif

#ifdef __cplusplus
}
#endif
