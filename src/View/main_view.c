#include "View/main_view.h"

#include <conio.h>
#include <math.h>
#include <stdlib.h>

#include "LinearAlgebra/physics.h"
#include "View/drawer.h"
#include "View/shaders.h"
#include "View/shape_stash.h"

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
Matrix yOpN;
Matrix op;

void freeOperator(Matrix* op) {
    for (int i = 0; i < 3; i++) {
        free(op->values[i]);
    }
    free(op->values);
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

    setUpOperator(&yOpL, generateYRotationOperator, 1.0f);
    setUpOperator(&yOpR, generateYRotationOperator, -1.0f);
    setUpOperator(&yOpN, generateYRotationOperator, .0f);
}

enum Action { JUMP = ' ', LEFT = 'a', RIGHT = 'd' };

void _evaluateAction() {
    char ch = getch();
    if (ch == RIGHT)
        op = yOpR;
    else if (ch == LEFT)
        op = yOpL;
    else if (ch == JUMP) {
        initializeJump();
        op = yOpN;
    }
}

void runMainLoop() {
    evaluateJump();
    drawCube(cube, shaderProgram);
    if (kbhit()) {
        _evaluateAction();
        rotateObserver(op);
    }
}

void runOnExit() {
    glDeleteProgram(shaderProgram);
    freeOperator(&yOpL);
    freeOperator(&yOpR);
    freeOperator(&yOpN);
}