//
//  Game.hpp
//  Mycraft
//
//  Created by Clapeysron on 14/11/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp
#include "opengl_header.h"
#include "ChunkData.hpp"
#include "Block.hpp"
enum Game_mode {
    NORMAL_MODE,
    GOD_MODE
};

class Game {
public:
    Game();
    VisibleChunks visibleChunks = VisibleChunks(8, 125,  8);
    Chunk chunk = Chunk(0, 0);
    Block block;
    Game_mode game_mode;
    glm::vec3 user_position;
    glm::vec3 steve_position;
    float vertical_v;
    bool move(glm::vec3 new_postions);
    bool gravity_move();
private:
    bool if_in_block(int x, int y, int z, glm::vec3 position);
};
#endif /* Game_hpp */
