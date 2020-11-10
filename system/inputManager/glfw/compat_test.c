#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#include <stdio.h>
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


    return 0;
}
