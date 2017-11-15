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
    void draw(glm::vec3 position, glm::mat4 view, glm::mat4 projection, Shader& Block_Shader, char type);
    ~Block();
private:
    unsigned int VBO, VAO;
};

#endif /* Block_hpp */
