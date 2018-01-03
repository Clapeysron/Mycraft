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
#include "BlockInfo.hpp"
enum Game_mode {
    NORMAL_MODE,
    GOD_MODE
};

enum Game_perspective {
    FIRST_PERSON,
    THIRD_PERSON
};

enum Options {
    FOV,
    CURSOR,
    GAME_MODE,
    GAME_PERSPECTIVE,
    ZOOM
};



class Game {
public:
    Game();
    VisibleChunks visibleChunks = VisibleChunks(8, 125, 8);
    Chunk chunk = Chunk(0, 0);
    Block block;
    Game_mode game_mode;
    Game_perspective game_perspective;
    Options game_options;
    glm::vec3 user_position;
    glm::vec3 steve_position;
    float vertical_v;
    bool move(glm::vec3 new_postions);
    bool trymove(glm::vec3 new_postions);
    bool steve_in_water();
    bool steve_eye_in_water();
    bool gravity_move(float deltaTime);
private:
    bool prev_in_water;
    bool if_in_block(int x, int y, int z, glm::vec3 position);
};
#endif /* Game_hpp */
