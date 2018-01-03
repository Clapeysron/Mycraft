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
#include "game_settings.h"
#include "BlockInfo.hpp"
#include <vector>

class Block {
public:
    float *tempVertex;
    Block();
    void updateBuffer(bool isNew, float *vertices, unsigned long size);
    void newHoldBlock();
    void updateBlock(char type);
    ~Block();
    unsigned int getVAO();
private:
    unsigned int VBO, VAO;
};

#endif /* Block_hpp */
