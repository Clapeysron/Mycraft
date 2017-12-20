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
#include "Gui.hpp"
#include <math.h>

#define SHADOW_WIDTH 12800
#define SHADOW_HEIGHT 12800

class Render
{
public:
    Render();
    void initial(Game& game);
    void render(Game& game);
    Sky Sky;
    Gui Gui;
    GLFWwindow* window;
    Shader Block_Shader;
    Shader Depth_Shader;
    Shader Depth_debug_Shader;
    Shader Steve_Shader;
    Model steve_model;
private:
    static int screen_width, screen_height;
    unsigned int texture_pic;
    unsigned int depthMap_fbo;
    unsigned int depthMap_pic;
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
    static bool tryRemove;
    static bool tryPlace;
    void texture_init();
    void depthMap_init();
    float steve_turn_angle(glm::vec3 cameraFront);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void processInput(GLFWwindow *window, Game& game);
};

#endif /* Render_h */
