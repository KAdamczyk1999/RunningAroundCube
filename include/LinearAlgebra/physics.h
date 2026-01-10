#pragma once

#include "Geometry/shapes.h"
#include "LinearAlgebra/lin_alg.h"

void rotateLightSource(Matrix op);

void initializeJump();

void evaluateJump();

Point getLightSource();

int compareObserverProx(const void* a, const void* b);

int compareRectsLightSourceProx(const void* a, const void* b);

typedef enum { CROUCHING_DOWN, STANDING_UP } CrouchingState;

void evaluateCrouch(Cube* cube, CrouchingState state);

typedef enum { DIR_FORWARD, DIR_BACKWARD, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN } MoveDirection;

void move(MoveDirection dir);
void rotate(MoveDirection dir);
