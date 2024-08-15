#include "View/main_view.h"

#include <math.h>

#include "View/drawer.h"
#include "View/shaders.h"

GLuint shaderProgram;

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
}

void _rotateObjects(Point pointOfRotation) {}

void runMainLoop() {}

void runOnExit() { glDeleteProgram(shaderProgram); }