// Lighting

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../includes/camera.h"
#include "../includes/shader.h"

void framebuffer_size_callback(GLFWwindow *, int, int);
void processInput(GLFWwindow *);
void keyboardCallback(GLFWwindow *, int, int, int, int);
void mouse_callback(GLFWwindow *, double, double);
void scroll_callback(GLFWwindow *, double, double);

// window settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;
const char *TITLE = "Colors & Lighting :: OpenGL";

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0f;
float lastY = (float)SCR_HEIGHT / 2.0f;
bool firstMouse = true;

bool moveForward = false;
bool moveBackward = false;
bool moveLeft = false;
bool moveRight = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // ..:: INIT WINDOW ::..
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // ..:: OPENGL CONTEXT ::..
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // ..:: OPENGL CONFIG ::..
    glEnable(GL_DEPTH_TEST);

    // ..:: SHADERS ::..
    Shader cubeShader("cube_vs.glsl", "cube_fs.glsl");
    Shader lightSourceShader("light_source_vs.glsl", "light_source_fs.glsl");

    // ..:: VERTICES ::..
    float vertices[] = {
        // position
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //
        0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
        -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
        -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, //
        -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f, //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, //
        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f, //
        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, //

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //
        0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, //
        0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f, //
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //
        0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
        -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, //
        0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
        -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f  //
    };

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    // only needs binding, no need to fill it again.
    // TODO: understand this magic
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // ..:: RENDER LOOP ::..
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // solid background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader and set uniform and drawing objects
        cubeShader.use();
        cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        cubeShader.setVec3("lightPos", lightPos);

        // view & projection transformation
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f, 100.0f);
        cubeShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        cubeShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        cubeShader.setMat4("model", model);

        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // draw lamp
        lightSourceShader.use();
        lightSourceShader.setMat4("projection", projection);
        lightSourceShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightSourceShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // ..:: CLEANUP ::..
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action,
                      int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        moveForward = true;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        moveForward = false;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        moveBackward = true;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        moveBackward = false;
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        moveLeft = true;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        moveLeft = false;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        moveRight = true;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        moveRight = false;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (moveForward)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (moveBackward)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (moveLeft)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (moveRight)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset =
        lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
