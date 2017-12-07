//
//  Block.cpp
//  Mycraft
//
//  Created by Clapeysron on 14/11/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "Block.hpp"

float vertex[QUAD_SIZE/VERTEX_SIZE][QUAD_SIZE] = {
    // x-
    0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    // x+
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    //z-
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    //z+
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    //y-
    0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    //y+
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
};

std::vector<float> Quads;

Block::Block() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    /*glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    Quads.clear();
    for(int i = 0; i < sizeof(test1)/sizeof(float); i++) {
        Quads.push_back(test1[i]);
    }
    glBufferData(GL_ARRAY_BUFFER, Quads.size()*sizeof(float), &Quads[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);*/
    //glBindVertexArray(VAO);
}

void Block::updateBuffer(bool isNew, float *vertex, unsigned long size) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if(isNew) {
        glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), vertex, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(6*sizeof(float)));
    }
    else {
        //glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), vertices, GL_STATIC_DRAW);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size*sizeof(float), vertex);
        /*void * ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr, vertices, sizeof(vertices));
        glUnmapBuffer(GL_ARRAY_BUFFER);*/
    }
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //glBindVertexArray(VAO);
}

Block::~Block() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Block::draw(glm::vec3 position, glm::mat4 view, glm::mat4 projection, Shader& Block_Shader, char type) {
    Block_Shader.use();
    glm::mat4 model(1);
    Block_Shader.setMat4("view", view);
    Block_Shader.setMat4("projection", projection);
    model = glm::translate(model, position);
    Block_Shader.setMat4("model", model);
    glBindVertexArray(VAO);
    if (type != 0) {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertex)/(VERTEX_SIZE*sizeof(float)));
    }
}

void Block::test(glm::mat4 view, glm::mat4 projection, Shader& Block_Shader) {
    Block_Shader.use();
    glm::mat4 model(1);
    Block_Shader.setMat4("view", view);
    Block_Shader.setMat4("projection", projection);
    //model = glm::translate(model, position);
    Block_Shader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, Quads.size()/VERTEX_SIZE);
}

unsigned int Block::getVAO(){
    return VAO;
}
