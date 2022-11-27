// These have to be in this order, don't change!
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>

#include "shader.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *TITLE = "OpenGL Shenanigans";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // load shaders
    Shader shader("shaders/vertex.vs", "shaders/fragment.fs");

    // ..:: Vertices ::..
    float vertices[] = {
        // position      //color
        0.0,  0.5,  0.0, 1.0f, 0.0f, 0.0f, // top
        -0.5, -0.5, 0.0, 0.0f, 1.0f, 0.0f, // bot left
        0.5,  -0.5, 0.0, 0.0f, 0.0f, 1.0f  // bot right
    };
    // float vertices[] = {
    //     0.5f,  0.5f,  0.0f, // top right
    //     0.5f,  -0.5f, 0.0f, // bottom right
    //     -0.5f, -0.5f, 0.0f, // bottom left
    //     -0.5f, 0.5f,  0.0f  // top left
    // };
    // unsigned int indices[] = {
    //     0, 1, 3, // first triangle
    //     1, 2, 3  // second triangle
    // };

    // generating a unique buffer IDs
    unsigned int VBO, VAO;
    // unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    // bind vertex array object first, then set vertex buffers and configure
    // vertex attribs
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
    //              GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(
        0,                 // corresponds to "location = 0" in the vertexShader
        3,                 // size of the vertex attribute (vec3 => 3 elements)
        GL_FLOAT,          // type of data in the vertex
        GL_FALSE,          // normalise input data?
        6 * sizeof(float), // stride
        (void *)0          // offset of position data in buffer
    );
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbind array buffer
    // Note: this is allowed, the call to glVertexAttribPointer registered VBO
    // as the vertex attribute's bound vertex buffer object so afterwards we can
    // safely unbind
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // (!!) do NOT unbind the EBO while a VAO is active as the bound element
    // buffer object IS stored in the VAO; keep EBO bound!
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Unbinding the VAO so other VAO calls do not accidentally modify this VAO
    // (even if this rarely happens, since modifying VAOs require a call to
    // glBindVertexArray anyway). As a general rule: don't unbind VAOs (nor
    // VBOs) when it is not necessary.
    // glBindVertexArray(0);

    // draw wireframe polygons
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ..:: RENDER LOOP ::..
    while (!glfwWindowShouldClose(window)) {
        // catch input
        processInput(window);

        // rendering loop
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0);; // no need to unbind it every time

        // end rendering loop
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);

    // terminate GLFW (this will also clean up all resources, making the
    // previous step optional)
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
