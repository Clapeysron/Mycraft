//
//  Render.h
//  Mycraft
//
//  Created by Clapeysron on 14/11/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#ifndef Render_h
#define Render_h
#include <iostream>
#include "opengl_header.h"
#include "game_settings.h"
#include "Game.hpp"
class Render
{
public:
    Render();
    void initial();
    void render(Game& game);
    GLFWwindow* window;
    glm::mat4 view;
    glm::mat4 projection;
    Shader Block_Shader;
private:
    static float deltaTime;
    float lastFrame;
    static glm::vec3 cameraPos;
    static glm::vec3 cameraFront;
    static glm::vec3 cameraUp;
    static bool firstMouse;
    static float yaw;
    static float pitch;
    static float fov;
    static float lastX;
    static float lastY;
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void processInput(GLFWwindow *window);
};

#endif /* Render_h */
