#include "View/main_view.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "LinearAlgebra/physics.h"
#include "View/drawer.h"
#include "View/shaders.h"
#include "View/shape_stash.h"

static void _delay(int millis) {
    int t = clock();
    while (clock() - t < millis);
}

GLuint shaderProgram;

void setUpOperator(Matrix* op, void (*f)(Matrix*, float), float angle) {
    op->values = (float**)malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) op->values[i] = (float*)malloc(3 * sizeof(float));
    op->columnCount = 3;
    op->rowCount = 3;

    f(op, angle);
}

Matrix yOpL;
Matrix yOpR;
Matrix xOpU;
Matrix xOpD;
const float baseMovementAngle = 2.0f;

void freeOperator(Matrix* op) {
    for (int i = 0; i < 3; i++) {
        free(op->values[i]);
    }
    free(op->values);
}

static void setupOperators() {
    setUpOperator(&xOpU, generateXRotationOperator, baseMovementAngle);
    setUpOperator(&xOpD, generateXRotationOperator, -baseMovementAngle);
    setUpOperator(&yOpL, generateYRotationOperator, baseMovementAngle);
    setUpOperator(&yOpR, generateYRotationOperator, -baseMovementAngle);
}

void runOnEntry() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glEnable(GL_COLOR_MATERIAL);

    setupOperators();
}

enum Action {
    CROUCH = 'C',
    JUMP = ' ',
    LEFT = 'A',
    RIGHT = 'D',
    BACK = 'S',
    FORWARD = 'W',
    ROTATE_RIGHT = 262,
    ROTATE_LEFT = 263,
    ROTATE_DOWN = 264,
    ROTATE_UP = 265,
};

int nonActionCounter = 0;
int const crouchStandUpDelay = 29;

void keyCallback(GLFWwindow*, int key, int, int, int) {
    nonActionCounter = 0;
    switch (key) {
        case CROUCH:
            evaluateCrouch(&cube, CROUCHING_DOWN);
            return;
        case RIGHT:
            move(DIR_RIGHT);
            break;
        case LEFT:
            move(DIR_LEFT);
            break;
        case JUMP:
            initializeJump();
            break;
        case BACK:
            move(DIR_BACKWARD);
            break;
        case FORWARD:
            move(DIR_FORWARD);
            break;
        case ROTATE_LEFT:
            rotate(DIR_LEFT);
            break;
        case ROTATE_RIGHT:
            rotate(DIR_RIGHT);
            break;
        case ROTATE_UP:
            rotate(DIR_UP);
            break;
        case ROTATE_DOWN:
            rotate(DIR_DOWN);
            break;
    }
    evaluateCrouch(&cube, STANDING_UP);
}

static void _evaluateAction() {
    if (nonActionCounter++ > crouchStandUpDelay) evaluateCrouch(&cube, STANDING_UP);
}

void runMainLoop() {
    _delay(20);
    evaluateJump();
    _evaluateAction();
    drawCube(cube, shaderProgram);
}

void runOnExit() {
    glDeleteProgram(shaderProgram);
    freeOperator(&yOpL);
    freeOperator(&yOpR);
}
