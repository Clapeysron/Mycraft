//
//  initialize.cpp
//  Mycraft
//
//  Created by Clapeysron on 14/11/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "Render.hpp"

bool Render::firstMouse = true;
float Render::yaw   =  -90.0f;
float Render::pitch =  0.0f;
float Render::fov   =  45.0f;
float Render::lastX =  800.0f / 2.0;
float Render::lastY =  600.0 / 2.0;
glm::vec3 Render::cameraPos   = glm::vec3(8.0f, 135.0f,  8.0f);
glm::vec3 Render::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Render::cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float Render::deltaTime = 0.0f;

Render::Render() {
    deltaTime = 0.0f;
    lastFrame = 0.0f;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
}

void Render::initial() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mycraft", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    Block_Shader = Shader("shader/Block.vs","shader/Block.fs");
}

void Render::render(Game& game) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput(window);
    projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    game.test.draw(cameraPos, view, projection, Block_Shader);
    /*char *ret = game.testchunk.readChunk();
    for(int i = 0; i < 256; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            for(int k = 0; k < 16; k++)
            {
                game.block.draw(glm::vec3(0.0f+j,0.0f+i,0.0f+k), view, projection, Block_Shader, ret[256*i+16*j+k]);
            }
        }
    }*/
    //game.block.test(view, projection, Block_Shader);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Render::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Render::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void Render::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed = 10.0 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        fov -= cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        fov += cameraSpeed;
}


