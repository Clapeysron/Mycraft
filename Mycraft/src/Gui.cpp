//
//  Gui.cpp
//  Mycraft
//
//  Created by Clapeysron on 17/12/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "Gui.hpp"

#define SIGHTBEAD_LEN 5.0f

float sightbead_vertex[] {
    -SIGHTBEAD_LEN, 1.0f,
    -SIGHTBEAD_LEN, -1.0f,
    SIGHTBEAD_LEN, -1.0f,
    SIGHTBEAD_LEN, -1.0f,
    -SIGHTBEAD_LEN, 1.0f,
    SIGHTBEAD_LEN, 1.0f,
    -1.0f, SIGHTBEAD_LEN,
    -1.0f, -SIGHTBEAD_LEN,
    1.0f, -SIGHTBEAD_LEN,
    1.0f, -SIGHTBEAD_LEN,
    -1.0f, SIGHTBEAD_LEN,
    1.0f, SIGHTBEAD_LEN
};

Gui::~Gui() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Gui::gui_init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sightbead_vertex), &sightbead_vertex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
}

void Gui::draw(int screen_width, int screen_height) {
    Gui_Shader.use();
    glm::mat4 model(1);
    model = glm::scale(model, glm::vec3(0.004f * screen_height / screen_width, 0.004f, 0.0f));
    Gui_Shader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);
}
