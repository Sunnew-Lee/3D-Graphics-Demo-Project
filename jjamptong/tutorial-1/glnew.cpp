// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#include "glnew.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>

#include <iostream>

unsigned int GLNew::VAO;
unsigned int GLNew::VBO;

GLFWwindow* GLNew::window;
unsigned int GLNew::shdrpgm;
const char* GLNew::vertShdrFile = 
"#version 330 core\n"
"layout (location = 0) in vec3 Position;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(vec3(Position.xyz), 1.0);\n"
"}\0";;
const char* GLNew::fragShdrFile = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";;
void GLNew::init()
{
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    window = glfwCreateWindow(800, 600, "jjamptong", nullptr, nullptr);
    
    glfwMakeContextCurrent(window);
    glViewport(0, 0, 800, 600);

    glewInit();

    GLuint vertShdr = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShdr, 1, &vertShdrFile, NULL);
    glCompileShader(vertShdr);

    unsigned int fragShdr = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShdr, 1, &fragShdrFile, NULL);
    glCompileShader(fragShdr);

    shdrpgm = glCreateProgram();
    glAttachShader(shdrpgm, vertShdr);
    glAttachShader(shdrpgm, fragShdr);
    glLinkProgram(shdrpgm);
    
    glDeleteShader(vertShdr);
    glDeleteShader(fragShdr);
}


void GLNew::update()
{
     std::array<glm::vec3, 3> pos_vtx{
     glm::vec3(-0.5f, 0.5f,0.f), glm::vec3(0.5f, 0.5f,0.f),
     glm::vec3(0.f, -0.5f,0.f)
     };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pos_vtx.size(), pos_vtx.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}


void GLNew::draw()
{
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shdrpgm);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void GLNew::cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shdrpgm);

    glfwTerminate();
}

