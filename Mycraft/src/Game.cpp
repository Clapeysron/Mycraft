//
//  Game.cpp
//  Mycraft
//
//  Created by Clapeysron on 14/11/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "Game.hpp"

Game::Game() {
    game_mode = NORMAL_MODE;
    vertical_v = 0;
    steve_position = glm::vec3(8.0f, 125.0f,  8.0f);
}

bool Game::move(glm::vec3 new_position) {
    int base_x = (int)(floor(new_position.x + 0.5)-1);
    int base_y = (int)(floor(new_position.y + 2 - STEVE_EYE_HEIGHT) -2);
    int base_z = (int)(floor(new_position.z + 0.5)-1);
    for (int dx=0; dx<2; dx++) {
        for (int dz=0; dz<2; dz++) {
            for (int dy=0; dy<3; dy++) {
                char type = visibleChunks.getBlockType(base_y+dy, base_x+dx, base_z+dz);
                if (type >> 7 == 0) {
                    if ( if_in_block(base_y+dy, base_x+dx, base_z+dz, new_position) == true ) return false;
                }
            }
        }
    }
    steve_position = new_position;
    //char type = visibleChunks.getBlockType(new_position.y, new_position.x, new_position.z);
    return true;
}

bool Game::if_in_block(int y, int x, int z, glm::vec3 position) {
    if ( (position.x-(x+1)) >= STEVE_RADIUS ) return false;
    if ( (x-position.x) >= STEVE_RADIUS ) return false;
    if ( (position.z-(z+1)) >= STEVE_RADIUS ) return false;
    if ( (z-position.z) >= STEVE_RADIUS ) return false;
    if ( (position.y-(y+1)) >= (STEVE_EYE_HEIGHT))  return false;
    if ( (y-position.y) >= ((STEVE_HEIGHT - STEVE_EYE_HEIGHT)+1) ) return false;
    return true;
}

bool Game::gravity_move() {
    vertical_v -= GRAVITY/1000;
    glm::vec3 new_position;
    new_position = steve_position + glm::vec3(0.0f, vertical_v, 0.0f);
    if ( !move(new_position) ) {
        vertical_v = 0;
        return false;
    }
    return true;
}
                        
                        
