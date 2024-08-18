#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "LinearAlgebra/lin_alg.h"

void setUpOperator(Matrix* op, void (*f)(Matrix*, float), float angle);
void freeOperator(Matrix* op);

void runOnEntry();
void runMainLoop();
void runOnExit();

#ifdef __cplusplus
}
#endif