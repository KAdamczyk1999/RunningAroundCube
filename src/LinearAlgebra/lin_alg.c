#include "LinearAlgebra/lin_alg.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void applyOperatorOn3dPoint(Matrix op, Point* point) {
    Point tempPoint;
    tempPoint.x = point->x * op.values[0][0] + point->y * op.values[0][1] + point->z * op.values[0][2];
    tempPoint.y = point->x * op.values[1][0] + point->y * op.values[1][1] + point->z * op.values[1][2];
    tempPoint.z = point->x * op.values[2][0] + point->y * op.values[2][1] + point->z * op.values[2][2];
    memcpy(point, &tempPoint, sizeof(Point));
}

void generateXRotationOperator(Matrix* matrix, float angle) {
    float angleRad = degsToRads(angle);
    matrix->values[0][0] = 1;
    matrix->values[0][1] = 0;
    matrix->values[0][2] = 0;
    matrix->values[1][0] = 0;
    matrix->values[1][1] = cos(angleRad);
    matrix->values[1][2] = -sin(angleRad);
    matrix->values[2][0] = 0;
    matrix->values[2][1] = sin(angleRad);
    matrix->values[2][2] = cos(angleRad);
}

void generateYRotationOperator(Matrix* matrix, float angle) {
    float angleRad = degsToRads(angle);
    matrix->values[0][0] = cos(angleRad);
    matrix->values[0][1] = 0;
    matrix->values[0][2] = sin(angleRad);
    matrix->values[1][0] = 0;
    matrix->values[1][1] = 1;
    matrix->values[1][2] = 0;
    matrix->values[2][0] = -sin(angleRad);
    matrix->values[2][1] = 0;
    matrix->values[2][2] = cos(angleRad);
}

void generateZRotationOperator(Matrix* matrix, float angle) {
    float angleRad = degsToRads(angle);
    matrix->values[0][0] = cos(angleRad);
    matrix->values[0][1] = -sin(angleRad);
    matrix->values[0][2] = 0;
    matrix->values[1][0] = sin(angleRad);
    matrix->values[1][1] = cos(angleRad);
    matrix->values[1][2] = 0;
    matrix->values[2][0] = 0;
    matrix->values[2][1] = 0;
    matrix->values[2][2] = 1;
}

float _calculateMag(Point p) { return sqrtf(powf(p.x, 2) + powf(p.y, 2) + powf(p.z, 2)); }

Point _substractPoints(Point p1, Point p2) {
    Point diff;
    diff.x = p1.x - p2.x;
    diff.y = p1.y - p2.y;
    diff.z = p1.z - p2.z;
    return diff;
}

float _dotProd(Point p1, Point p2) {
    float prod = .0f;
    prod += p1.x * p2.x;
    prod += p1.y * p2.y;
    prod += p1.z * p2.z;
    return prod;
}

Point _multiplyPoints(Point p1, Point p2) {
    Point prod;
    prod.x = p1.y * p2.z - p1.z * p2.y;
    prod.y = p1.z * p2.x - p1.x * p2.z;
    prod.z = p1.x * p2.y - p1.y * p2.x;
    return prod;
}

float _calculateLineAngle(Point* lines) {
    float lineMags[2];
    for (int i = 0; i < 2; i++) lineMags[i] = _calculateMag(lines[i]);
    return acos(_dotProd(lines[0], lines[1]) / (lineMags[0] * lineMags[1]));
}

Point _normalize(Point point) {
    float mag = _calculateMag(point);
    point.x /= mag;
    point.y /= mag;
    point.z /= mag;
    return point;
}

Point _generateRandomPoint() {
    Point point;
    point.x = (float)rand();
    point.y = (float)rand();
    point.z = (float)rand();
    return point;
}

void _findOthrogonalPoints(Point* points, Point point) {
    assert(point.x != .0f && point.y != .0f && "Plane vector has to have non-zero coeffs!");
    point = _normalize(point);
    Point anyPoint = _generateRandomPoint();
    anyPoint = _normalize(anyPoint);

    points[0] = _multiplyPoints(point, anyPoint);
    points[0] = _normalize(points[0]);

    points[1] = _multiplyPoints(point, points[0]);
    points[1] = _normalize(points[1]);
}

Point _calculateProjection(Point point, Point* plainPoints) {
    float u1DotProd = _dotProd(plainPoints[0], plainPoints[0]);
    float u2DotProd = _dotProd(plainPoints[1], plainPoints[1]);

    float mult1 = _dotProd(point, plainPoints[0]) / u1DotProd;
    float mult2 = _dotProd(point, plainPoints[1]) / u2DotProd;

    Point ret;
    ret.x = plainPoints[0].x * mult1 + plainPoints[1].x * mult2;
    ret.y = plainPoints[0].y * mult1 + plainPoints[1].y * mult2;
    ret.z = plainPoints[0].z * mult1 + plainPoints[1].z * mult2;

    return ret;
}

float calculateDist(Point p1, Point p2) {
    return sqrtf(powf(p1.x - p2.x, 2.0f) + powf(p1.y - p2.y, 2.0f) + powf(p1.z - p2.z, 2.0f));
}

void projectRect(Rect* rect, Point pointOfView) {
    Point plainPoints[2];
    _findOthrogonalPoints(plainPoints, pointOfView);
    for (int i = 0; i < 4; i++) {
        rect->vertices[i] = _calculateProjection(rect->vertices[i], plainPoints);
    }
}

float calculateRectProjectionArea(Rect rect, Point pointOfView) {
    projectRect(&rect, pointOfView);
    float ds[2];
    Point lines[2];
    const int parameterCount = sizeof(ds) / sizeof(ds[0]);
    for (int i = 0; i < parameterCount; i++) {
        ds[i] = calculateDist(rect.vertices[i], rect.vertices[i + 2]);
        lines[i] = _substractPoints(rect.vertices[i], rect.vertices[i + 2]);
    }

    return ds[0] * ds[1] * sin(_calculateLineAngle(lines)) * .5f;
}