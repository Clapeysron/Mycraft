//
//  Cube.cpp
//  MineCraft
//
//  Created by apple on 17/10/22.
//  Copyright © 2017年 apple. All rights reserved.
//

#include "Cube.hpp"

void grassCube::translate(glm::vec3 trans)
{
    model = glm::translate(model, trans);
}

void grassCube::setTexture(char *sideImage, char *topImage, char *bottomImage)
{
}
