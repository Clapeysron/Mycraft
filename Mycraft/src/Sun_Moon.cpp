//
//  Sun_Moon.cpp
//  Mycraft
//
//  Created by Clapeysron on 26/12/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "Sun_Moon.hpp"

float sunMoonVertices[] = {
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.1f, 1.0f,
    1.0f, 0.0f, 0.1f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.1f, 1.0f
};

void Sun_Moon::Sun_Moon_init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sunMoonVertices), &sunMoonVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
    
    glGenTextures(1, &Sun_Moon_Shader_pic);
    glBindTexture(GL_TEXTURE_2D, Sun_Moon_Shader_pic);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    unsigned char *data = stbi_load_out("picture/Sun_Moon.png", &width, &height, &nrChannels, STBI_rgb_alpha_out);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "****** Failed to load texture ******" << std::endl;
    }
    stbi_image_free_out(data);
}

void Sun_Moon::draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model) {
    glDepthFunc(GL_LEQUAL);
    Sun_Moon_Shader.use();
    Sun_Moon_Shader.setMat4("view", view);
    Sun_Moon_Shader.setMat4("model", model);
    Sun_Moon_Shader.setMat4("projection", projection);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Sun_Moon_Shader_pic);
    glDrawArrays(GL_TRIANGLES, 0, 8);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}


Sun_Moon::~Sun_Moon() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
