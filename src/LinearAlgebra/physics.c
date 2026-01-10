#include "LinearAlgebra/physics.h"

#include <math.h>
#include <stdbool.h>

typedef enum {
    ANGLE_CORRECTION_APPLIED,
    ANGLE_CORRECTION_REVERSED,
} AngleCorrectionType;

#define BASE_COORD 1e-6
const float initialJumpingVelocity = .05f;
const float crouchingVelocity = .05;
const float crouchingFinalPosition = -0.3;
const float gravitationalAcceleration = .003f;

const float baseMovementVelocity = .15f;
extern const float baseMovementAngle;

float observerVerticalVelocity = 0.0f;
float observerVirtualLatitude = BASE_COORD;
float observerVirtualAngle = 0.0f;

extern Cube cube;
extern Matrix yOpL;
extern Matrix yOpR;
extern Matrix xOpU;
extern Matrix xOpD;

static int _cmpf(float val1, float val2) {
    if (val1 > val2)
        return 1;
    else if (val1 == val2)
        return 0;
    else
        return -1;
}

static float _calculateMean(float* vals, int valCount) {
    float sum = .0f;
    for (int i = 0; i < valCount; i++) sum += vals[i];
    return sum / valCount;
}

Point observer = {BASE_COORD, BASE_COORD, BASE_COORD};

int compareObserverProx(const void* a, const void* b) {
    Rect* rectA = (Rect*)a;
    Rect* rectB = (Rect*)b;
    float valsA[4] = {calculateDist(rectA->vertices[0], observer), calculateDist(rectA->vertices[1], observer),
                      calculateDist(rectA->vertices[2], observer), calculateDist(rectA->vertices[3], observer)};
    float valsB[4] = {calculateDist(rectB->vertices[0], observer), calculateDist(rectB->vertices[1], observer),
                      calculateDist(rectB->vertices[2], observer), calculateDist(rectB->vertices[3], observer)};
    return _cmpf(_calculateMean(valsB, 4), _calculateMean(valsA, 4));
}

Point lightSource = {2.0f, 2.0f, 4.0f};

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

static bool isRotationValid(MoveDirection dir) {
    if (dir == DIR_UP && observerVirtualAngle < -90.0f) return false;
    if (dir == DIR_DOWN && observerVirtualAngle > 90.0f) return false;
    return true;
}

static void correctAngle(AngleCorrectionType type) {
    float angle = type == ANGLE_CORRECTION_APPLIED ? observerVirtualAngle : -observerVirtualAngle;
    rotatePointX(&lightSource, angle);
    for (uint8_t i = 0; i < CUBE_VERTICES_COUNT; i++) rotatePointX(&cube.vertices[i], angle);
}

void rotate(MoveDirection dir) {
    if (!isRotationValid(dir)) return;
    Matrix rotOp;
    switch (dir) {
        case DIR_LEFT:
            rotOp = yOpL;
            break;
        case DIR_RIGHT:
            rotOp = yOpR;
            break;
        case DIR_UP:
            rotOp = xOpU;
            observerVirtualAngle -= baseMovementAngle;
            break;
        case DIR_DOWN:
            rotOp = xOpD;
            observerVirtualAngle += baseMovementAngle;
            break;
    }
    bool requiresCorrection = dir == DIR_RIGHT || dir == DIR_LEFT;
    if (requiresCorrection) correctAngle(ANGLE_CORRECTION_APPLIED);
    applyOperatorOn3dPoint(rotOp, &lightSource);
    for (uint8_t i = 0; i < CUBE_VERTICES_COUNT; i++) applyOperatorOn3dPoint(rotOp, &cube.vertices[i]);
    if (requiresCorrection) correctAngle(ANGLE_CORRECTION_REVERSED);
}

void move(MoveDirection dir) {
    int8_t dirSign = (dir == DIR_BACKWARD || dir == DIR_LEFT) ? 1 : -1;
    float velocity = baseMovementVelocity * dirSign;
    if (dir == DIR_FORWARD || dir == DIR_BACKWARD) {
        float observerAngleRads = degsToRads(observerVirtualAngle);
        float vZ = velocity * cos(observerAngleRads);
        float vY = velocity * sin(observerAngleRads);
        for (uint8_t i = 0; i < CUBE_VERTICES_COUNT; i++) {
            cube.vertices[i].z += vZ;
            cube.vertices[i].y += vY;
        }
        lightSource.z += vZ;
        lightSource.y += vY;
    } else {
        for (uint8_t i = 0; i < CUBE_VERTICES_COUNT; i++) cube.vertices[i].x += velocity;
        lightSource.x += velocity;
    }
}

void initializeJump() {
    if (observerVirtualLatitude <= BASE_COORD && observerVirtualLatitude >= -BASE_COORD)
        observerVerticalVelocity = initialJumpingVelocity;
}

void evaluateJump() {
    if (observerVerticalVelocity == 0) return;
    correctAngle(ANGLE_CORRECTION_APPLIED);
    observerVirtualLatitude += observerVerticalVelocity;
    lightSource.y -= observerVerticalVelocity;
    for (uint8_t i = 0; i < CUBE_VERTICES_COUNT; i++) cube.vertices[i].y -= observerVerticalVelocity;
    observerVerticalVelocity -= gravitationalAcceleration;
    if (observerVirtualLatitude <= BASE_COORD) {
        lightSource.y += observerVirtualLatitude;
        for (uint8_t i = 0; i < CUBE_VERTICES_COUNT; i++) cube.vertices[i].y += observerVirtualLatitude;
        observerVirtualLatitude = BASE_COORD;
        observerVerticalVelocity = 0.0f;
    }
    correctAngle(ANGLE_CORRECTION_REVERSED);
}

static void _evaluateCrouchDown() {
    if (observerVerticalVelocity != .0f) return;
    if (observerVirtualLatitude >= BASE_COORD) return;
    if (observerVirtualLatitude < crouchingFinalPosition) return;
    observerVirtualLatitude -= crouchingVelocity;
    correctAngle(ANGLE_CORRECTION_APPLIED);
    lightSource.y += crouchingVelocity;
    for (uint8_t i = 0; i < CUBE_VERTICES_COUNT; i++) cube.vertices[i].y += crouchingVelocity;
    correctAngle(ANGLE_CORRECTION_REVERSED);
}

static void _evaluateStandUp() {
    if (observerVerticalVelocity != .0f) return;
    if (observerVirtualLatitude > .0f) {
        for (uint8_t i = 0; i < CUBE_VERTICES_COUNT; i++) cube.vertices[i].y += observerVirtualLatitude;
        observerVirtualLatitude = BASE_COORD;
        return;
    }
    observerVirtualLatitude += crouchingVelocity;
    correctAngle(ANGLE_CORRECTION_APPLIED);
    lightSource.y -= crouchingVelocity;
    for (uint8_t i = 0; i < CUBE_VERTICES_COUNT; i++) cube.vertices[i].y -= crouchingVelocity;
    correctAngle(ANGLE_CORRECTION_REVERSED);
}

void evaluateCrouch(Cube* cube, CrouchingState state) {
    if (state == STANDING_UP)
        _evaluateStandUp();
    else if (state == CROUCHING_DOWN)
        _evaluateCrouchDown();
}
