#include <GLFW/glfw3.h>
#include <stdio.h>
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);


    GLFWwindow * window = glfwCreateWindow(640, 480, "COMPAT_TEST", NULL, NULL);
    glfwMakeContextCurrent(window);



    printf("GL_VERSION  : %s\n", glGetString(GL_VERSION));
    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER));

    glViewport(0, 0, 640, 480);

    return 0;
}
