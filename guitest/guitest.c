#include <GL/glfw.h>

int main()
{
    int ret = glfwInit();
    if (ret == GL_TRUE)
    {
        glfwTerminate();
        return 0;
    }
    return 1;
}