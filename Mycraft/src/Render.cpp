//
//  initialize.cpp
//  Mycraft
//
//  Created by Clapeysron on 14/11/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "Render.hpp"
#include "Stbi_load.hpp"

bool Render::firstMouse = true;
float Render::yaw   =  -90.0f;
float Render::pitch =  0.0f;
float Render::fov   =  45.0f;
float Render::lastX =  800.0f / 2.0;
float Render::lastY =  600.0 / 2.0;
float Render::deltaTime = 0.0f;
int Render::screen_width = SCREEN_WIDTH*2;
int Render::screen_height = SCREEN_HEIGHT*2;
glm::vec3 Render::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Render::cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

Render::Render() {
    lastFrame = 0.0f;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(screen_width/2, screen_height/2, "Mycraft", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void Render::initial(Game &game) {
    
    view = glm::lookAt(game.steve_position, game.steve_position + cameraFront, cameraUp);
    projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
    Block_Shader = Shader("shader/Block.vs", "shader/Block.fs");
    Sky.Sky_init();
    Sky.Sky_Shader = Shader("shader/Skybox.vs", "shader/Skybox.fs");
    texture_init();
    Depth_Shader = Shader("shader/Depth.vs", "shader/Depth.fs");
    depthMap_init();
    Depth_debug_Shader = Shader("shader/Depth_debug.vs", "shader/Depth_debug.fs");
}

void Render::depthMap_init() {
    glGenFramebuffers(1, &depthMap_fbo);
    glGenTextures(1, &depthMap_pic);
    glBindTexture(GL_TEXTURE_2D, depthMap_pic);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMap_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_pic, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Render::texture_init() {
    glGenTextures(1, &texture_pic);
    glBindTexture(GL_TEXTURE_2D, texture_pic);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    unsigned char *data = stbi_load_out("picture/texture_big.png", &width, &height, &nrChannels, STBI_rgb_alpha_out);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "****** Failed to load texture ******" << std::endl;
    }
    stbi_image_free_out(data);
}

// FOR DEBUG !
GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
    if (quadVAO == 0)
    {
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
            1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Render::render(Game& game) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    printf("==========================\n");
    printf("fps: %.2f\n",1.0f/deltaTime);
    printf("view x:%.2f y:%.2f z:%.2f", cameraFront.x, cameraFront.y, cameraFront.z);
    processInput(window, game);
    if (game.game_mode == NORMAL_MODE) {
        game.gravity_move();
    }

    // depth scene
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    glm::vec3 lightPos(8.0f, 127.0f, 8.0f);
    GLfloat near_plane = 1.0f, far_plane = 20.0f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProjection * lightView;
    Depth_Shader.use();
    Depth_Shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMap_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    game.visibleChunks.drawDepth(Depth_Shader, texture_pic);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Chunks render
    glViewport(0, 0, screen_width, screen_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
    view = glm::lookAt(game.steve_position, game.steve_position + cameraFront, cameraUp);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    game.visibleChunks.draw(game.steve_position, view, projection, Block_Shader, texture_pic, depthMap_pic, lightSpaceMatrix, lightPos);
    
    // depth shadow draw DEBUG
    Depth_debug_Shader.use();
    Depth_debug_Shader.setFloat("near_plane", near_plane);
    Depth_debug_Shader.setFloat("far_plane", far_plane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap_pic);
    //RenderQuad();
    
    // Draw sky box
    Sky.draw(game.steve_position, view, projection);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Render::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    screen_width = width;
    screen_height = height;
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
    if (pitch > 89.9f)
        pitch = 89.9f;
    if (pitch < -89.9f)
        pitch = -89.9f;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void Render::processInput(GLFWwindow *window, Game &game)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    float cameraSpeed = 6.0 * deltaTime;
    glm::vec3 cameraFront_XZ = cameraFront;
    glm::vec3 new_position;
    cameraFront_XZ.y = 0;
    cameraFront_XZ = glm::normalize(cameraFront_XZ);
    glm::vec3 cameraRight_XZ = glm::cross(cameraFront, cameraUp);
    cameraRight_XZ.y = 0;
    cameraRight_XZ = glm::normalize(cameraRight_XZ);
    glm::vec3 cameraFront_Y = glm::vec3(0.0f, 1.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        new_position = game.steve_position + cameraSpeed * glm::vec3(cameraFront_XZ.x, 0.0f, 0.0f);
        game.move(new_position);
        new_position = game.steve_position + cameraSpeed * glm::vec3(0.0, 0.0f, cameraFront_XZ.z);
        game.move(new_position);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        new_position = game.steve_position - cameraSpeed * glm::vec3(cameraFront_XZ.x, 0.0f, 0.0f);
        game.move(new_position);
        new_position = game.steve_position - cameraSpeed * glm::vec3(0.0, 0.0f, cameraFront_XZ.z);
        game.move(new_position);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        new_position = game.steve_position - cameraSpeed * glm::vec3(cameraRight_XZ.x, 0.0f, 0.0f);
        game.move(new_position);
        new_position = game.steve_position - cameraSpeed * glm::vec3(0.0, 0.0f, cameraRight_XZ.z);
        game.move(new_position);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        new_position = game.steve_position + cameraSpeed * glm::vec3(cameraRight_XZ.x, 0.0f, 0.0f);
        game.move(new_position);
        new_position = game.steve_position + cameraSpeed * glm::vec3(0.0, 0.0f, cameraRight_XZ.z);
        game.move(new_position);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        new_position = game.steve_position + cameraSpeed/10 * glm::vec3(cameraFront_XZ.x, 0.0f, 0.0f);
        game.move(new_position);
        new_position = game.steve_position + cameraSpeed/10 * glm::vec3(0.0, 0.0f, cameraFront_XZ.z);
        game.move(new_position);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        new_position = game.steve_position - cameraSpeed/10 * glm::vec3(cameraFront_XZ.x, 0.0f, 0.0f);
        game.move(new_position);
        new_position = game.steve_position - cameraSpeed/10 * glm::vec3(0.0, 0.0f, cameraFront_XZ.z);
        game.move(new_position);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        new_position = game.steve_position - cameraSpeed/10 * glm::vec3(cameraRight_XZ.x, 0.0f, 0.0f);
        game.move(new_position);
        new_position = game.steve_position - cameraSpeed/10 * glm::vec3(0.0, 0.0f, cameraRight_XZ.z);
        game.move(new_position);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        new_position = game.steve_position + cameraSpeed/10 * glm::vec3(cameraRight_XZ.x, 0.0f, 0.0f);
        game.move(new_position);
        new_position = game.steve_position + cameraSpeed/10 * glm::vec3(0.0, 0.0f, cameraRight_XZ.z);
        game.move(new_position);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (game.game_mode == NORMAL_MODE) {
            if (game.vertical_v == 0) {
                game.vertical_v = JUMP_V/19.0f ;
            }
        } else {
            new_position = game.steve_position + cameraSpeed * cameraFront_Y;
            game.move(new_position);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        new_position = game.steve_position - cameraSpeed * cameraFront_Y;
        game.move(new_position);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        fov -= cameraSpeed*10;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        game.game_mode = GOD_MODE;
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        game.game_mode = NORMAL_MODE;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        fov += cameraSpeed*10;
    }
}


