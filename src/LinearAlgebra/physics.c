#include "LinearAlgebra/physics.h"

#include <math.h>

#define BASE_COORD 1e-6
const float initialJumpingVelocity = .05f;
const float crouchingVelocity = .05;
const float crouchingFinalPosition = -0.3;
const float gravitationalAcceleration = .003f;
float observerVerticalVelocity = 0.0f;

int _cmpf(float val1, float val2) {
    if (val1 > val2)
        return 1;
    else if (val1 == val2)
        return 0;
    else
        return -1;
}

float _calculateMean(float* vals, int valCount) {
    float sum = .0f;
    for (int i = 0; i < valCount; i++) sum += vals[i];
    return sum / valCount;
}

Point observer = {BASE_COORD, BASE_COORD, 1.0f};
Point getObserver() { return observer; }

void rotateObserver(Matrix op) { applyOperatorOn3dPoint(op, &observer); }

float getObserverAngle() {
    float angle =
        observer.z * observer.x > 0 ? fabs(atan(observer.z / observer.x)) : fabs(atan(observer.x / observer.z));
    if (observer.x < 0.0f && observer.z < 0.0f)
        return angle + PI;
    else if (observer.x > 0.0f && observer.z < 0.0f)
        return angle + 3 * PI / 2;
    else if (observer.x < 0.0f && observer.z > 0.0f)
        return angle + PI / 2;
    else
        return angle;
}

int compareObserverProx(const void* a, const void* b) {
    Rect* rectA = (Rect*)a;
    Rect* rectB = (Rect*)b;
    float valsA[4] = {calculateDist(rectA->vertices[0], observer), calculateDist(rectA->vertices[1], observer),
                      calculateDist(rectA->vertices[2], observer), calculateDist(rectA->vertices[3], observer)};
    float valsB[4] = {calculateDist(rectB->vertices[0], observer), calculateDist(rectB->vertices[1], observer),
                      calculateDist(rectB->vertices[2], observer), calculateDist(rectB->vertices[3], observer)};
    return _cmpf(_calculateMean(valsB, 4), _calculateMean(valsA, 4));
}

void initializeJump() {
    if (observer.y <= BASE_COORD && observer.y >= -BASE_COORD) observerVerticalVelocity = initialJumpingVelocity;
}

void evaluateJump() {
    if (observerVerticalVelocity == 0) return;
    observer.y += observerVerticalVelocity;
    observerVerticalVelocity -= gravitationalAcceleration;
    if (observer.y <= BASE_COORD) {
        observer.y = BASE_COORD;
        observerVerticalVelocity = 0.0f;
    }
}

void _evaluateCrouchDown() {
    if (observer.y > BASE_COORD) return;
    observer.y -= crouchingVelocity;
    if (observer.y < crouchingFinalPosition) observer.y = crouchingFinalPosition;
}

void _evaluateStandUp() {
    if (observer.y >= BASE_COORD) return;
    observer.y += crouchingVelocity;
    if (observer.y > BASE_COORD) observer.y = BASE_COORD;
}

void evaluateCrouch(CrouchingState state) {
    if (state == STANDING_UP)
        _evaluateStandUp();
    else if (state == CROUCHING_DOWN)
        _evaluateCrouchDown();
}
const Point lightSource = {2.0f, 2.0f, 4.0f};
Point getLightSource() { return lightSource; }

int compareRectsLightSourceProx(const void* a, const void* b) {
    Rect* rectA = (Rect*)a;
    Rect* rectB = (Rect*)b;
    float valsA[4] = {calculateDist(rectA->vertices[0], lightSource), calculateDist(rectA->vertices[1], lightSource),
                      calculateDist(rectA->vertices[2], lightSource), calculateDist(rectA->vertices[3], lightSource)};
    float valsB[4] = {calculateDist(rectB->vertices[0], lightSource), calculateDist(rectB->vertices[1], lightSource),
                      calculateDist(rectB->vertices[2], lightSource), calculateDist(rectB->vertices[3], lightSource)};
    return _cmpf(_calculateMean(valsA, 4), _calculateMean(valsB, 4));
}