#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <array>
#include <cstdlib>
#include "Ball.hpp"
#include "Wall.hpp"
#include "Config.h"

const char *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * vec4(aPos, 0.0, 1.0);\n"
"}\0";

const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\0";

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::array<Wall, 2> *walls = (std::array<Wall, 2>*) glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS){
        switch (key) {
            case GLFW_KEY_W:{
                walls->at(0).up = true;
                walls->at(0).down = false;
                break;
            }
            case GLFW_KEY_S:{
                walls->at(0).down = true;
                walls->at(0).up = false;
                break;
            }
            case GLFW_KEY_UP:{
                walls->at(1).up = true;
                walls->at(1).down = false;
                break;
            }
            case GLFW_KEY_DOWN:{
                walls->at(1).down = true;
                walls->at(1).up = false;
                break;
            }
        }
    }
    else{
         switch (key) {
            case GLFW_KEY_W:{
                walls->at(0).up = false;
                break;
            }
            case GLFW_KEY_S:{
                walls->at(0).down = false;
                break;
            }
            case GLFW_KEY_UP:{
                walls->at(1).up = false;
                break;
            }
            case GLFW_KEY_DOWN:{
                walls->at(1).down = false;
                break;
            }
        }
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ping Pong", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGL();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glm::mat4 projection = glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f);
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    std::vector<GLfloat> vertices{
    };

    std::vector<GLushort> indices = {
    };

    std::array<Wall, 2> walls{
        Wall(64.0f, static_cast<float>(WINDOW_HEIGHT) / 2 - Wall::HEIGHT / 2, vertices, indices),
        Wall(static_cast<float>(WINDOW_WIDTH) - 64.0f - Wall::WIDTH, static_cast<float>(WINDOW_HEIGHT) / 2 - Wall::HEIGHT / 2, vertices, indices)
    };
    Ball ball(static_cast<float>(WINDOW_WIDTH) / 2, static_cast<float>(WINDOW_HEIGHT) / 2, vertices, walls);

    glfwSetWindowUserPointer(window, &walls);
    glfwSetKeyCallback(window, keyCallback);
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1000, indices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 1000, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

    glBindVertexArray(0);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        ball.appendPosition();
        

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
        walls[0].appendPosition(true);
        walls[1].appendPosition(false);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}