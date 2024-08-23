#pragma once

#include "Geometry/shapes.h"
#include "LinearAlgebra/lin_alg.h"

void rotateObserver(Matrix op);

void initializeJump();

void evaluateJump();

Point getObserver();

float getObserverAngle();

int compareObserverProx(const void* a, const void* b);

Point getLightSource();

int compareRectsLightSourceProx(const void* a, const void* b);