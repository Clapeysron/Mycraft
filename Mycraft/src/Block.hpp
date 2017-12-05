//
//  Block.hpp
//  Mycraft
//
//  Created by Clapeysron on 14/11/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#ifndef Block_hpp
#define Block_hpp

#include "opengl_header.h"
#include <vector>

class Block {
public:
    Block();
    void updateBuffer(bool isNew, float *vertices, unsigned long size);
    void draw(glm::vec3 position, glm::mat4 view, glm::mat4 projection, Shader& Block_Shader, char type);
    void test(glm::mat4 view, glm::mat4 projection, Shader& Block_Shader);
    ~Block();
    unsigned int getVAO();
private:
    unsigned int VBO, VAO;
};

#endif /* Block_hpp */
