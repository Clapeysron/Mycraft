//
//  main.cpp
//  Mycraft
//
//  Created by Clapeysron on 14/11/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include <iostream>
#include "opengl_header.h"
#include "game_settings.h"
#include "Render.hpp"
#include "Block.hpp"
#include "Game.hpp"


int main(int argc, const char * argv[]) {
    Render render;
    Game game;
    render.initial(game);
    game.chunk.generateMap();
    while (!glfwWindowShouldClose(render.window)) {
        render.render(game);
    }
    glfwTerminate();
}


