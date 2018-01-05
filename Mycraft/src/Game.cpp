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
    game_perspective = FIRST_PERSON;
    game_options = FOV;
    vertical_v = 0;
    steve_position = glm::vec3(8.0f, 125.0f,  8.0f);
}

bool Game::move(glm::vec3 new_position) {
    if (game_mode == GOD_MODE) {
        steve_position = new_position;
        return true;
    } else {
        int base_x = (int)(floor(new_position.x + 0.5)-1);
        int base_y = (int)(floor(new_position.y + 2 - STEVE_EYE_HEIGHT) -2);
        int base_z = (int)(floor(new_position.z + 0.5)-1);
        for (int dx=0; dx<2; dx++) {
            for (int dz=0; dz<2; dz++) {
                for (int dy=0; dy<3; dy++) {
                    char type = visibleChunks.getBlockType(base_y+dy, base_x+dx, base_z+dz);
                    if ( ((unsigned char)type < 0xc0 || (unsigned char)type > 0xc9) && ((unsigned char)type > 0xf2 || (unsigned char)type < 0xf0)) {
                        if ( if_in_block(base_y+dy, base_x+dx, base_z+dz, new_position) == true ) return false;
                    }
                }
            }
        }
        steve_position = new_position;
        return true;
    }
}

bool Game::trymove(glm::vec3 new_position) {
    if (game_mode == GOD_MODE) {
        steve_position = new_position;
        return true;
    } else {
        int base_x = (int)(floor(new_position.x + 0.5)-1);
        int base_y = (int)(floor(new_position.y + 2 - STEVE_EYE_HEIGHT) -2);
        int base_z = (int)(floor(new_position.z + 0.5)-1);
        for (int dx=0; dx<2; dx++) {
            for (int dz=0; dz<2; dz++) {
                for (int dy=0; dy<3; dy++) {
                    char type = visibleChunks.getBlockType(base_y+dy, base_x+dx, base_z+dz);
                    if ( ((unsigned char)type < 0xc0 || (unsigned char)type > 0xc9) && ((unsigned char)type > 0xf2 || (unsigned char)type < 0xf0)) {
                        if ( if_in_block(base_y+dy, base_x+dx, base_z+dz, new_position) == true ) return false;
                    }
                }
            }
        }
        return true;
    }
}

bool Game::if_in_block(int y, int x, int z, glm::vec3 position) {
    if ( (position.x-(x+1)) > STEVE_RADIUS ) return false;
    if ( (x-position.x) > STEVE_RADIUS ) return false;
    if ( (position.z-(z+1)) > STEVE_RADIUS ) return false;
    if ( (z-position.z) > STEVE_RADIUS ) return false;
    if ( position.x>(x+1) && position.z>(z+1) && glm::length(glm::vec2(position.x-(x+1), position.z-(z+1))) > STEVE_RADIUS) return false;
    if ( position.x>(x+1) && position.z<z && glm::length(glm::vec2(position.x-(x+1), z-position.z)) > STEVE_RADIUS) return false;
    if ( position.x<x && position.z>(z+1) && glm::length(glm::vec2(x-position.x, position.z-(z+1))) > STEVE_RADIUS) return false;
    if ( position.x<x && position.z<z && glm::length(glm::vec2(x-position.x, z-position.z)) > STEVE_RADIUS) return false;
    if ( (position.z-(z+1)) > STEVE_RADIUS ) return false;
    if ( (z-position.z) > STEVE_RADIUS ) return false;
    if ( (position.y-(y+1)) > (STEVE_EYE_HEIGHT))  return false;
    if ( (y-position.y) > (STEVE_HEIGHT - STEVE_EYE_HEIGHT)) return false;
    return true;
}

bool Game::steve_in_water() {
    return (visibleChunks.getBlockType(floor(steve_position.y - STEVE_EYE_HEIGHT), floor(steve_position.x), floor(steve_position.z)) == (char)WATER);
}

bool Game::steve_eye_in_water() {
    return (visibleChunks.getBlockType(floor(steve_position.y), floor(steve_position.x), floor(steve_position.z)) == (char)WATER);
}

bool Game::gravity_move(float deltaTime) {
    bool now_in_water = steve_in_water();
    if (now_in_water && !prev_in_water) {
        vertical_v = vertical_v * 0.5;
    } else if (prev_in_water && !now_in_water) {
        vertical_v += JUMP_V * 1.5;
    } else if (now_in_water) {
        vertical_v -= (MAX_WATER_V + vertical_v)/15 * deltaTime*GRAVITY;
    } else {
        vertical_v -= deltaTime*GRAVITY*3;
    }
    prev_in_water = now_in_water;
    glm::vec3 new_position;
    new_position = steve_position + deltaTime * glm::vec3(0.0f, vertical_v, 0.0f);
    if ( !move(new_position) ) {
        vertical_v = 0;
        return false;
    }
    return true;
}
                        
                        
