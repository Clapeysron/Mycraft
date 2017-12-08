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
#include "Sky.hpp"

class Render
{
public:
    Render();
    void initial(Game& game);
    void render(Game& game);
    Sky Sky;
    GLFWwindow* window;
    Shader Block_Shader;
   
private:
    unsigned int texture_pic;
    static float deltaTime;
    float lastFrame;
    glm::mat4 view;
    glm::mat4 projection;
    static float frustumPlanes[6][4];
    static glm::vec3 cameraFront;
    static glm::vec3 cameraUp;
    static bool firstMouse;
    static float yaw;
    static float pitch;
    static float fov;
    static float lastX;
    static float lastY;
    void texture_init();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void processInput(GLFWwindow *window, Game& game);
};

#endif /* Render_h */
