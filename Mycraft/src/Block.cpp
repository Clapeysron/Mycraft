//
//  Block.cpp
//  Mycraft
//
//  Created by Clapeysron on 14/11/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "Block.hpp"

/*float vertex[FACE_TYPE_NUM][QUAD_SIZE] = {
    // x-
    0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.1f,
    0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.1f, 0.1f,
    0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.1f, 0.0f,
    0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.1f,
    0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.1f, 0.0f,
    // x+
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.1f, 0.1f,
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.1f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.1f, 0.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.1f, 0.1f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    //z-
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.1f, 0.1f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.1f, 0.0f,
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.1f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.1f, 0.1f,
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    //z+
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.1f,
    0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.1f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.1f,
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.0f,
    //y-
    0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.1f, 0.1f,
    0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.1f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.1f,
    0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.1f, 0.1f,
    1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    //y+
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.1f,
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.1f, 0.0f,
    1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.1f, 0.1f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.1f,
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.1f, 0.0f,
    
    0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.1f,
    0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.1f, 0.1f,
    0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.1f, 0.0f,
    0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.1f,
    0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.1f, 0.0f,
    
    //z_center
    0.0f, 0.0f, 0.5f, 0.0f, 0.0f, -1.0f, 0.1f, 0.1f,
    0.0f, 1.0f, 0.5f, 0.0f, 0.0f, -1.0f, 0.1f, 0.0f,
    1.0f, 1.0f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.1f,
    0.0f, 0.0f, 0.5f, 0.0f, 0.0f, -1.0f, 0.1f, 0.1f,
    1.0f, 1.0f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f
};*/

float vertex[FACE_TYPE_NUM][QUAD_SIZE] = {
    // x-
    0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    // x+
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    //z-
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    //z+
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT, 
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    //y-
    0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    //y+
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    
    // x_center
    GRASS_MIN, 0.0f, GRASS_MAX, -1.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    GRASS_MIN, 1.0f, GRASS_MAX, -1.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    GRASS_MAX, 1.0f, GRASS_MIN, -1.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    GRASS_MAX, 0.0f, GRASS_MIN, -1.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    GRASS_MIN, 0.0f, GRASS_MAX, -1.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    GRASS_MAX, 1.0f, GRASS_MIN, -1.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    
    //z_center
    GRASS_MIN, 0.0f, GRASS_MIN, 1.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MAX, 1.0f, 0.0625f,
    GRASS_MIN, 1.0f, GRASS_MIN, 1.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MIN, 1.0f, 0.0625f,
    GRASS_MAX, 1.0f, GRASS_MAX, 1.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MIN, 1.0f, 0.0625f,
    GRASS_MAX, 0.0f, GRASS_MAX, 1.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MAX, 1.0f, 0.0625f,
    GRASS_MIN, 0.0f, GRASS_MIN, 1.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MAX, 1.0f, 0.0625f,
    GRASS_MAX, 1.0f, GRASS_MAX, 1.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MIN, 1.0f, 0.0625f
};


float alternative[FACE_TYPE_NUM][QUAD_SIZE] = {
    // x-
    0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    // x+
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    //z-
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    //z+
    0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    //y-
    1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    //y+
    1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    
    // x_center
    GRASS_MIN, 0.0f, GRASS_MAX, -1.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    GRASS_MIN, 1.0f, GRASS_MAX, -1.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    GRASS_MAX, 1.0f, GRASS_MIN, -1.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    GRASS_MAX, 0.0f, GRASS_MIN, -1.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    GRASS_MIN, 0.0f, GRASS_MAX, -1.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MAX, CORNER_SHADOW, POINT_LIGHT,
    GRASS_MAX, 1.0f, GRASS_MIN, -1.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MIN, CORNER_SHADOW, POINT_LIGHT,
    
    //z_center
    GRASS_MIN, 0.0f, GRASS_MIN, 1.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MAX, 1.0f, 0.0625f,
    GRASS_MIN, 1.0f, GRASS_MIN, 1.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MIN, 1.0f, 0.0625f,
    GRASS_MAX, 1.0f, GRASS_MAX, 1.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MIN, 1.0f, 0.0625f,
    GRASS_MAX, 0.0f, GRASS_MAX, 1.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MAX, 1.0f, 0.0625f,
    GRASS_MIN, 0.0f, GRASS_MIN, 1.0f, 0.0f, -1.0f, TEXTURE_MAX, TEXTURE_MAX, 1.0f, 0.0625f,
    GRASS_MAX, 1.0f, GRASS_MAX, 1.0f, 0.0f, -1.0f, TEXTURE_MIN, TEXTURE_MIN, 1.0f, 0.0625f
};

float waterVertices[QUAD_SIZE] = {
    0.0f, WATER_OFFSET, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.1f, 1.0f, 0.0625f,
    0.0f, WATER_OFFSET, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0625f,
    1.0f, WATER_OFFSET, 1.0f, 0.0f, 1.0f, 0.0f, 0.1f, 0.0f, 1.0f, 0.0625f,
    1.0f, WATER_OFFSET, 0.0f, 0.0f, 1.0f, 0.0f, 0.1f, 0.1f, 1.0f, 0.0625f,
    0.0f, WATER_OFFSET, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.1f, 1.0f, 0.0625f,
    1.0f, WATER_OFFSET, 1.0f, 0.0f, 1.0f, 0.0f, 0.1f, 0.0f, 1.0f, 0.0625f
};

std::vector<float> Quads;

Block::Block() {
    tempVertex = (float*)malloc(sizeof(vertex));
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

void Block::newHoldBlock() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Block::updateBlock(char type) {
    memcpy(tempVertex, vertex, sizeof(vertex));
    BlockInfo blockInfo = BlockInfoMap[type];
    for(int m = 0; m < 6*QUAD_SIZE; m = m+VERTEX_SIZE)
    {
        tempVertex[m+6] += blockInfo.x_pos_x;
        tempVertex[m+7] += blockInfo.x_pos_y;
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), tempVertex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Block::updateBuffer(bool isNew, float *vertex, unsigned long size) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if(isNew) {
        glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), vertex, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(3*sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(6*sizeof(float)));
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(8*sizeof(float)));
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(9*sizeof(float)));
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
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindVertexArray(VAO);
}

Block::~Block() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

unsigned int Block::getVAO(){
    return VAO;
}
