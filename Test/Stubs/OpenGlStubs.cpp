#include <GLFW/glfw3.h>

int glfwInit() { return 1; }
void glfwWindowHint(int hint, int value) {}
GLFWwindow* window;
GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
    uint8_t address[1];
    window = (GLFWwindow*)address;
    return window;
}
void glfwTerminate(void) {}
void glfwMakeContextCurrent(GLFWwindow* window) {}
GLFWglproc proc;
GLFWglproc glfwGetProcAddress(const char* procname) { return proc; }
int glfwWindowShouldClose(GLFWwindow* window) { return 1; }
void glfwPollEvents(void) {}
void glfwSwapBuffers(GLFWwindow* window) {}