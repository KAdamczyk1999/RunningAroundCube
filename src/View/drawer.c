#include "View/drawer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Common/types.h"
#include "LinearAlgebra/physics.h"
#include "View/main_view.h"

#define PROPERTIES_MAX_COUNT 36000

static void _setUpShape(Shape shape, int shapeIndex, GLuint* VAO, GLuint* VBO) {
    GLfloat shapeProperties[PROPERTIES_MAX_COUNT];

    glBindVertexArray(VAO[shapeIndex]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[shapeIndex]);

    int totalVals = DIMENTION_COUNT + COLOR_COUNT;

    int neededWords = shape.verticesCount * totalVals;
    assert(neededWords <= PROPERTIES_MAX_COUNT && "Too many vertices in one shape!");
    int neededSpace = shape.verticesCount * sizeof(GLfloat) * totalVals;

    mapShapeToGLVertices(shape, shapeProperties, DIMENTION_COUNT);

    glBufferData(GL_ARRAY_BUFFER, neededSpace, shapeProperties, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, DIMENTION_COUNT, GL_FLOAT, GL_FALSE, totalVals * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, COLOR_COUNT, GL_FLOAT, GL_FALSE, totalVals * sizeof(GLfloat),
                          (void*)(COLOR_COUNT * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

static void _drawShape(Shape shape, int shapeIndex, GLuint* VAO) {
    glBindVertexArray(VAO[shapeIndex]);
    glDrawArrays(shape.drawingMethod, 0, shape.verticesCount);
}

void drawShapeArray(Shape* shapes, int shapeCount, GLuint shaderProgram) {
    GLuint VAO[PROPERTIES_MAX_COUNT / (DIMENTION_COUNT + COLOR_COUNT)];
    GLuint VBO[PROPERTIES_MAX_COUNT / (DIMENTION_COUNT + COLOR_COUNT)];
    glGenVertexArrays(shapeCount, VAO);
    glGenBuffers(shapeCount, VBO);

    for (int i = 0; i < shapeCount; i++) {
        _setUpShape(shapes[i], i, VAO, VBO);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(shaderProgram);
    for (int i = 0; i <= shapeCount; i++) {
        _drawShape(shapes[i], i, VAO);
    }
    glUseProgram(0);

    glDeleteVertexArrays(shapeCount, VAO);
    glDeleteBuffers(shapeCount, VBO);
}

void drawCircle(Circle circle, GLuint shaderProgram) {
#define STEPS 50
    Shape shapes[STEPS + 1];
    const float angle = 360.0f / STEPS;
    Point prevPoint = {circle.centerPoint.x, circle.centerPoint.y - circle.radius};

    for (int i = 0; i <= STEPS; i++) {
        Point newPoint = prevPoint;
        rotatePoint(&newPoint, angle, circle.centerPoint);
        Point shapeVertices[3] = {circle.centerPoint, {prevPoint.x, prevPoint.y, 0.0f}, {newPoint.x, newPoint.y, 0.0f}};
        shapes[i].vertices = (Point*)malloc(sizeof(shapeVertices));
        memcpy(shapes[i].vertices, shapeVertices, sizeof(shapeVertices));
        shapes[i].verticesCount = 3;
        shapes[i].drawingMethod = GL_TRIANGLES;
        shapes[i].color = circle.color;

        prevPoint = newPoint;
    }

    drawShapeArray(shapes, STEPS + 1, shaderProgram);

    for (int i = 0; i <= STEPS; i++) {
        free(shapes[i].vertices);
    }
}

void drawRect(Rect rect, GLuint shaderProgram) {
    Point triangleVertices[2][3];
    Shape triangles[2];
    for (int i = 0; i < 2; i++) {
        triangleVertices[i][0] = rect.vertices[0];
        triangleVertices[i][1] = rect.vertices[2];
        triangleVertices[i][2] = rect.vertices[1 + i * 2];
        triangles[i].vertices = triangleVertices[i];
        triangles[i].verticesCount = 3;
        triangles[i].drawingMethod = GL_TRIANGLES;
        triangles[i].color = rect.color;
    }
    drawShapeArray(triangles, 2, shaderProgram);
}

#define CUBE_RECTS_COUNT 6
static void _lightRects(Rect* rects) {
    for (int i = 0; i < CUBE_RECTS_COUNT; i++) {
        float colorVals = .3f;
        if (i < CUBE_RECTS_COUNT / 2) {
            float area = calculateRectProjectionArea(rects[i], getLightSource());
            colorVals += area / calculateRectArea(rects[i]) * colorVals;
        }
        rects[i].color = (Color){.r = colorVals, .g = colorVals, .b = colorVals};
    }
}

static void _rotateRectsToCamera(Rect* rects) {
    Matrix yOp;
    setUpOperator(&yOp, generateYRotationOperator, getObserverAngle() / PI * 180.f + 90.0f);
    for (int i = 0; i < CUBE_RECTS_COUNT; i++)
        for (int j = 0; j < 4; j++) applyOperatorOn3dPoint(yOp, &(rects[i].vertices[j]));
    freeOperator(&yOp);
}

static void _scaleCube(Cube* cube) {
    Point observer = getObserver();
    float observerProx = observer.x * observer.x + observer.z * observer.z;
    float scale = 1.0f / observerProx;
    for (int i = 0; i < sizeof(Cube) / sizeof(Point); i++) {
        cube->vertices[i].x *= scale;
        cube->vertices[i].y *= scale;
        cube->vertices[i].z *= scale;
    }
}

void drawCube(Cube cube, GLuint shaderPorgram) {
    _scaleCube(&cube);
    Rect rects[CUBE_RECTS_COUNT];
    memcpy(rects[0].vertices, cube.vertices, 4 * sizeof(Point));
    memcpy(rects[1].vertices, &(cube.vertices[4]), 4 * sizeof(Point));

    for (int i = 0; i < 4; i++) {
        memcpy(rects[i + 2].vertices, &(cube.vertices[i]), 2 * sizeof(Point));
        memcpy(&(rects[i + 2].vertices[2]), &(cube.vertices[4 + i + 1]), sizeof(Point));
        memcpy(&(rects[i + 2].vertices[3]), &(cube.vertices[4 + i]), sizeof(Point));
    }

    rects[5].vertices[0] = cube.vertices[0];
    rects[5].vertices[1] = cube.vertices[3];
    rects[5].vertices[2] = cube.vertices[7];
    rects[5].vertices[3] = cube.vertices[4];

    qsort(rects, CUBE_RECTS_COUNT, sizeof(Rect), compareRectsLightSourceProx);
    _lightRects(rects);

    qsort(rects, CUBE_RECTS_COUNT, sizeof(Rect), compareObserverProx);
    for (int i = 0; i < CUBE_RECTS_COUNT; i++) projectRect(&rects[i], getObserver());
    _rotateRectsToCamera(rects);
    for (int i = 0; i < CUBE_RECTS_COUNT; i++) drawRect(rects[i], shaderPorgram);
}
