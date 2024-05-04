/**
 * @Author: Xia Yunkai
 * @Date:   2024-05-04 00:53:51
 * @Last Modified by:   Xia Yunkai
 * @Last Modified time: 2024-05-04 14:42:15
 */
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "basis/defines.h"
#include "basis/logger.h"
#include <string>
using namespace std;


int main(int argc, char const *argv[])
{
    GLFWwindow *winfow = NULL; // 窗口句柄
    if (!glfwInit())
    {
        LOG_ERROR("glfw init failed");
        return -1;
    }
    winfow = glfwCreateWindow(640, 480, "Hello, world!", nullptr, nullptr);

    if (!winfow)
    {
        glfwTerminate();
    }

    // 创建上下文
    glfwMakeContextCurrent(winfow);
    if (glewInit() != GLEW_OK)
    {
        LOG_ERROR("glew init failed");
        glfwTerminate();
        return -1;
    }

    LOG_INFO("GL_VERSION: %s\n", glGetString(GL_VERSION));

    float positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f};
    // 创建buff
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);

    LOG_INFO("buffer: %d\n", buffer);

    while (!glfwWindowShouldClose(winfow))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(winfow);
        glfwPollEvents();
    }

    return 0;
}
