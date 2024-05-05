/**
 * @Author: Xia Yunkai
 * @Date:   2024-05-04 00:53:51
 * @Last Modified by:   Xia Yunkai
 * @Last Modified time: 2024-05-05 16:47:37
 */
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "basis/defines.h"
#include "basis/logger.h"
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct ShaderSourses
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderSourses
ParseShader(const std::string &filepath)
{
    std::fstream file(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };
    ShaderType type = ShaderType::NONE;

    std::stringstream ss[2];
    // 逐行读取文件
    std::string line;
    while (getline(file, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return {ss[0].str(), ss[1].str()};
}
static unsigned int CompileShader(const std::string &source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // alloca分配的是栈区内存，程序自动释放；
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        LOG_ERROR("failed to compile shader %s ,%s", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
        glDeleteShader(id);
        id = 0;
    }

    return id;
}

static int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

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

    LOG_INFO("GL_VERSION: %s", glGetString(GL_VERSION));

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

    ShaderSourses shaderSourses = ParseShader("res/shader/basic.shader");
    LOG_INFO("vertex: %s", shaderSourses.vertexSource.c_str());
    LOG_INFO("fragment: %s", shaderSourses.fragmentSource.c_str());
    unsigned int shader = CreateShader(shaderSourses.vertexSource, shaderSourses.fragmentSource);
    glUseProgram(shader);

    LOG_INFO("buffer: %d", buffer);

    while (!glfwWindowShouldClose(winfow))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(winfow);
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();

    return 0;
}
