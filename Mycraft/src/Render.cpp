//
//  initialize.cpp
//  Mycraft
//
//  Created by Clapeysron on 14/11/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "Render.hpp"
#include "Stbi_load.hpp"
//#define TIMETEST
//#define DEPTHTEST
#define SHADOW_MAPPING

bool Render::firstMouse = true;
float Render::yaw   =  -90.0f;
float Render::pitch =  0.0f;
float Render::fov   =  45.0f;
float Render::lastX =  800.0f / 2.0;
float Render::lastY =  600.0 / 2.0;
float Render::deltaTime = 0.0f;
bool Render::tryPlace = false;
bool Render::mouseHold = false;
int Render::screen_width = (SCREEN_WIDTH > 4096) ? 4096 : SCREEN_WIDTH;
int Render::screen_height = (SCREEN_HEIGHT > 2064) ? 2064: SCREEN_HEIGHT;
glm::vec3 Render::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Render::cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

Render::Render() {
    dayTime = 16.0f;
    removeCount = 0;
    //srand(0);
    srand((unsigned)time(NULL));
    randomSunDirection = fmod(rand(), 2*M_PI);
    //printf("randomSunDirection: %.2f\n",randomSunDirection)
    lastFrame = 0.0f;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, "Mycraft", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
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
    Block_Shader.use();
    Block_Shader.setInt("texture_pic", 0);
    Block_Shader.setInt("shadowMap", 1);
    Block_Shader.setInt("skybox", 2);
    Steve_Shader = Shader("shader/Steve.vs", "shader/Steve.fs");
    Sky.Sky_init();
    Sky.Sky_Shader = Shader("shader/Skybox.vs", "shader/Skybox.fs");
    Sun.Sun_init();
    Sun.Sun_Moon_Shader = Shader("shader/Sun_Moon.vs", "shader/Sun_Moon.fs");
    Sun.Sun_Moon_Shader.setInt("Sun_Moon_texture", 0);
    Sun.Sun_Moon_Shader.setInt("Star_texture", 1);
    Moon.Moon_init();
    Moon.Sun_Moon_Shader = Shader("shader/Sun_Moon.vs", "shader/Sun_Moon.fs");
    Moon.Sun_Moon_Shader.setInt("Sun_Moon_texture", 0);
    Moon.Sun_Moon_Shader.setInt("Star_texture", 1);
    Gui.gui_init();
    Gui.Gui_Shader = Shader("shader/Gui.vs", "shader/Gui.fs");
    texture_init();
    Depth_Shader = Shader("shader/Depth.vs", "shader/Depth.fs");
    depthMap_init();
    Depth_debug_Shader = Shader("shader/Depth_debug.vs", "shader/Depth_debug.fs");
    Depth_debug_Shader.setInt("depthMap", 0);
    steve_model = Model("model/alex.obj");
}

void Render::depthMap_init() {
    glGenFramebuffers(1, &depthMap_fbo);
    glGenTextures(1, &depthMap_pic);
    glBindTexture(GL_TEXTURE_2D, depthMap_pic);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
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
#ifdef TIMETEST
    float timeMark;
    timeMark = glfwGetTime();
    float totalTimeMark;
    totalTimeMark = glfwGetTime();
#endif
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    dayTime += deltaTime/10;
    dayTime = (dayTime>24) ? dayTime - 24 : dayTime;
    float starIntensity = calStarIntensity(dayTime);
    Sun_Moon_light = calLight(dayTime);
    Ambient_light = calAmbient(dayTime);
    printf("==========================\n");
    printf("Time: %d:%d\n", (int)floor(dayTime), (int)floor((dayTime-floor(dayTime))*60));
    printf("fps: %.2f\n",1.0f/deltaTime);
    printf("view x:%.2f y:%.2f z:%.2f\n", cameraFront.x, cameraFront.y, cameraFront.z);
    printf("pos x:%.2f y:%.2f z:%.2f\n", game.steve_position.x, game.steve_position.y, game.steve_position.z);
    printf("steve_in_water: %d\neye_in_water:%d\n", game.steve_in_water(), game.steve_eye_in_water());
    std::cout << "now_block:" << BlockInfoMap[game.visibleChunks.getBlockType(game.steve_position.y, game.steve_position.x, game.steve_position.z)].block_name << endl;
    
    processInput(window, game);
    
#ifdef TIMETEST
    printf("Process Input draw: %f\n", timeMark - glfwGetTime());
    timeMark = glfwGetTime();
#endif
    
    if (game.game_mode == NORMAL_MODE) {
        game.gravity_move(deltaTime);
    }
    
#ifdef TIMETEST
    printf("Gravity move: %f\n", timeMark - glfwGetTime());
    timeMark = glfwGetTime();
#endif
    glm::vec3 chosen_block_pos = game.visibleChunks.accessibleBlock(game.steve_position, cameraFront);
    char chosen_block_type = game.visibleChunks.getBlockType(chosen_block_pos.y, chosen_block_pos.x, chosen_block_pos.z);
    float broken_scale = 0;
    if(mouseHold && chosen_block_pos==prev_block_pos) {
        removeCount += deltaTime;
        float broke_time;
        if (game.game_mode == GOD_MODE) {
            broke_time = 0.3;
        } else {
            broke_time = BlockInfoMap[chosen_block_type].broke_time;
        }
        broken_scale = removeCount/broke_time;
        if (removeCount < broke_time) {
        } else {
            game.visibleChunks.removeBlock(game.steve_position, cameraFront);
            removeCount = 0;
        }
    } else {
        removeCount = 0;
    }
    
    std::cout << "removeCount:" << removeCount << endl;
    std::cout << "broken_scale" << broken_scale << endl;
    
    prev_block_pos = chosen_block_pos;
    if(tryPlace){
        bool ret = game.visibleChunks.placeBlock(game.steve_position, cameraFront, TORCH);
        tryPlace = false;
    }
    
#ifdef TIMETEST
    printf("Remove / Place block: %f\n", timeMark - glfwGetTime());
    timeMark = glfwGetTime();
#endif
    bool isDaylight = (dayTime >= 5.5 && dayTime <= 18.5);
    float shadowRadius = (RADIUS*2+1)*8;
    float dayTheta = (dayTime-SUNRISE_TIME)*M_PI/12;
    // depth scene
    glm::mat4 lightProjection, lightView, lightSpaceMatrix;
    glm::vec3 lightDirection;
    if (isDaylight) {
        lightDirection = glm::vec3(sin(randomSunDirection)*cos(dayTheta), -sin(dayTheta), cos(randomSunDirection)*cos(dayTheta));
    } else {
        lightDirection = glm::vec3(-sin(randomSunDirection)*cos(dayTheta), sin(dayTheta), -cos(randomSunDirection)*cos(dayTheta));
    }
    glm::mat4 depth_steve_model(1);
    glm::vec3 lightPos = game.steve_position;
    lightPos.y = 120.0f + shadowRadius*sin(dayTheta);
    lightPos.x += -shadowRadius*sin(randomSunDirection)*cos(dayTheta);
    lightPos.z += -shadowRadius*cos(randomSunDirection)*cos(dayTheta);
    glm::vec3 moonPos = game.steve_position;
    moonPos.y = 120.0f - shadowRadius*sin(dayTheta);
    moonPos.x += shadowRadius*sin(randomSunDirection)*cos(dayTheta);
    moonPos.z += shadowRadius*cos(randomSunDirection)*cos(dayTheta);
    
#ifdef SHADOW_MAPPING
    GLfloat near_plane = 0.0f, far_plane = shadowRadius + 512.0f;
    float orthoRatio = 1.2f;
    if (isDaylight) {
        lightProjection = glm::ortho(-shadowRadius*orthoRatio, shadowRadius*orthoRatio, -shadowRadius*orthoRatio, shadowRadius*orthoRatio, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, lightPos + lightDirection, glm::vec3(lightDirection.x, lightDirection.z, -lightDirection.y/(tan(dayTheta)*tan(dayTheta))));
        lightSpaceMatrix = lightProjection * lightView;
        Depth_Shader.use();
        Depth_Shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMap_fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        Depth_Shader.setMat4("model", glm::mat4(1));
        game.visibleChunks.drawDepth(Depth_Shader, texture_pic);
        if (game.game_perspective == THIRD_PERSON) {
            depth_steve_model = glm::translate(depth_steve_model, game.steve_position);
            depth_steve_model = glm::translate(depth_steve_model, glm::vec3(0, -STEVE_HEIGHT+0.1, 0));
            depth_steve_model = glm::rotate(depth_steve_model, cal_angle(cameraFront), glm::vec3(0, 1, 0));
            depth_steve_model = glm::scale(depth_steve_model, glm::vec3(0.3f * STEVE_HEIGHT));
            Depth_Shader.setMat4("model", depth_steve_model);
            steve_model.Draw(Depth_Shader);
        }
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
#endif
    
#ifdef TIMETEST
    printf("Depth scene draw: %f\n", timeMark - glfwGetTime());
    timeMark = glfwGetTime();
#endif
    
    // Chunks render
    glViewport(0, 0, screen_width, screen_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
    if (game.game_perspective == FIRST_PERSON) {
        view = glm::lookAt(game.steve_position, game.steve_position + cameraFront, cameraUp);
    } else {
        view = glm::lookAt(game.steve_position - glm::vec3(5.0f)*cameraFront, game.steve_position + cameraFront, cameraUp);
    }
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    if (game.game_perspective == THIRD_PERSON) {
        game.visibleChunks.draw(game.steve_position - glm::vec3(5.0f)*cameraFront, view, projection, Block_Shader, texture_pic, depthMap_pic, Sky.Skybox_pic, lightSpaceMatrix, lightDirection, chosen_block_pos, broken_scale, Sun_Moon_light, Ambient_light, isDaylight, dayTime, starIntensity, game.steve_eye_in_water());
    } else {
        game.visibleChunks.draw(game.steve_position, view, projection, Block_Shader, texture_pic, depthMap_pic, Sky.Skybox_pic, lightSpaceMatrix, lightDirection, chosen_block_pos, broken_scale, Sun_Moon_light, Ambient_light, isDaylight, dayTime, starIntensity, game.steve_eye_in_water());
    }
    // steve render
    if (game.game_perspective == THIRD_PERSON) {
        Steve_Shader.use();
        Steve_Shader.setMat4("projection", projection);
        Steve_Shader.setMat4("view", view);
        Steve_Shader.setVec3("sunlight.lightDirection", lightDirection);
        Steve_Shader.setVec3("sunlight.ambient", Ambient_light);
        Steve_Shader.setVec3("sunlight.lightambient", Sun_Moon_light);
        depth_steve_model = glm::mat4(1);
        depth_steve_model = glm::translate(depth_steve_model, game.steve_position);
        depth_steve_model = glm::translate(depth_steve_model, glm::vec3(0, -STEVE_HEIGHT+0.1, 0));
        depth_steve_model = glm::rotate(depth_steve_model, cal_angle(cameraFront), glm::vec3(0, 1, 0));
        depth_steve_model = glm::scale(depth_steve_model, glm::vec3(0.3f * STEVE_HEIGHT));
        Steve_Shader.setMat4("model", depth_steve_model);
        steve_model.Draw(Steve_Shader);
    }
#ifdef TIMETEST
    printf("Chunk scene draw: %f\n", timeMark - glfwGetTime());
    timeMark = glfwGetTime();
#endif
    
#ifdef DEPTHTEST
    // depth shadow draw DEBUG
    Depth_debug_Shader.use();
    Depth_debug_Shader.setFloat("near_plane", near_plane);
    Depth_debug_Shader.setFloat("far_plane", far_plane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap_pic);
    glViewport(0, 0, 1024, 1024);
    RenderQuad();
#ifdef TIMETEST
    printf("Depth Debug draw: %f\n", timeMark - glfwGetTime());
    timeMark = glfwGetTime();
#endif
#endif
    
    // Draw sky box
    glViewport(0, 0, screen_width, screen_height);
    Sky.draw(game.steve_position, view, projection, dayTime, starIntensity);
    
#ifdef TIMETEST
    printf("Skybox scene draw: %f\n", timeMark - glfwGetTime());
    timeMark = glfwGetTime();
#endif
    
    // Draw Sun or Moon
    glm::mat4 sun_model(1);
    sun_model = glm::translate(sun_model, lightPos);
    sun_model = glm::scale(sun_model, glm::vec3(60.f, 60.f, 60.f));
    //glm::vec3 axis = glm::cross(lightDirection ,glm::vec3(0,0,1));
    //float sun_theta = glm::angle(lightDirection, glm::vec3(0,0,1));
    sun_model = glm::rotate(sun_model, atan(lightDirection.x/lightDirection.z) + (float)M_PI, glm::vec3(0,1,0));
    sun_model = glm::rotate(sun_model, (randomSunDirection >= M_PI/2 && randomSunDirection <= M_PI*3/2) ? dayTheta : -dayTheta, glm::vec3(1,0,0));
    if (isDaylight) Sun.draw(view, projection, sun_model, dayTime, true);
    
    glm::mat4 moon_model(1);
    moon_model = glm::translate(moon_model, moonPos);
    moon_model = glm::scale(moon_model, glm::vec3(30.f, 30.f, 30.f));
    //glm::vec3 axis = glm::cross(lightDirection ,glm::vec3(0,0,1));
    //float sun_theta = glm::angle(lightDirection, glm::vec3(0,0,1));
    moon_model = glm::rotate(moon_model, atan(lightDirection.x/lightDirection.z) + (float)M_PI, glm::vec3(0,1,0));
    moon_model = glm::rotate(moon_model, (randomSunDirection >= M_PI/2 && randomSunDirection <= M_PI*3/2) ? dayTheta : -dayTheta, glm::vec3(1,0,0));
    Moon.draw(view, projection, moon_model, dayTime, false);
    
#ifdef TIMETEST
    printf("Sun scene draw: %f\n", timeMark - glfwGetTime());
    timeMark = glfwGetTime();
#endif
    
    // Draw gui
    Gui.draw(screen_width, screen_height);
    
#ifdef TIMETEST
    printf("Gui scene draw: %f\n", timeMark - glfwGetTime());
    timeMark = glfwGetTime();
#endif
    
    glfwSwapBuffers(window);
    
#ifdef TIMETEST
    printf("Swap Buffer time: %f\n", timeMark - glfwGetTime());
    timeMark = glfwGetTime();
#endif
    
    glfwPollEvents();
    
#ifdef TIMETEST
    printf("Depth Debug draw: %f\n", timeMark - glfwGetTime());
    printf("Total time: %f\n", totalTimeMark - glfwGetTime());
#endif
}

float Render::cal_angle(glm::vec3 cameraFront) {
    if (cameraFront.z < 0) {
        return atan(cameraFront.x/cameraFront.z) + M_PI;
    } else {
        return atan(cameraFront.x/cameraFront.z);
    }
}

float Render::calStarIntensity(float dayTime) {
    if (dayTime >=5 && dayTime <= 6) {
        return sin((6-dayTime)*M_PI/2);
    } else if (dayTime<5 || dayTime>19) {
        return 1.0f;
    } else if (dayTime >= 18 && dayTime <= 19) {
        return sin((dayTime-18)*M_PI/2);
    } else {
        return 0.0f;
    }
}

glm::vec3 Render::calLight(float dayTime) {
    glm::vec3 dayLight(0.9f);
    glm::vec3 nightLight(0.1f);
    if (dayTime >=5.5 && dayTime <= 6.5) {
        float dayIntensity = sin((6.5-dayTime)*M_PI/2);
        return dayIntensity*nightLight + (1-dayIntensity)*dayLight;
    } else if (dayTime<5.5 || dayTime>19) {
        return nightLight;
    } else if (dayTime >= 17 && dayTime <= 19) {
        float dayIntensity = sin((dayTime-17)*M_PI/4);
        return dayIntensity*nightLight + (1-dayIntensity)*dayLight;
    } else {
        return dayLight;
    }
}

glm::vec3 Render::calAmbient(float dayTime) {
    glm::vec3 dayLight(0.35f);
    glm::vec3 nightLight(0.1f);
    if (dayTime >=5.5 && dayTime <= 6.5) {
        float dayIntensity = sin((6.5-dayTime)*M_PI/2);
        return dayIntensity*nightLight + (1-dayIntensity)*dayLight;
    } else if (dayTime<5.5 || dayTime>19) {
        return nightLight;
    } else if (dayTime >= 17 && dayTime <= 19) {
        float dayIntensity = sin((dayTime-17)*M_PI/4);
        return dayIntensity*nightLight + (1-dayIntensity)*dayLight;
    } else {
        return dayLight;
    }
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

void Render::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    mouseHold = false;
    if (action == GLFW_RELEASE) {
        switch(button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                mouseHold = false;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                tryPlace = true;
                break;
            default:
                break;
        }
    }
    
    if (action == GLFW_PRESS) {
        switch(button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                mouseHold = true;
                break;
            default:
                mouseHold = false;
                break;
        }
    }
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
            if (game.steve_in_water()) {
                game.vertical_v += (game.vertical_v>0) ? (FLOAT_UP_V - game.vertical_v) : FLOAT_UP_V;
            } else if (game.vertical_v == 0 && !game.trymove(game.steve_position-glm::vec3(0, 0.02, 0))) {
                game.vertical_v = JUMP_V * 3;
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
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        game.game_perspective = FIRST_PERSON;
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        game.game_perspective = THIRD_PERSON;
    }
}
