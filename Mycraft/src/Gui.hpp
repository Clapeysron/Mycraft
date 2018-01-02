//
//  Gui.hpp
//  Mycraft
//
//  Created by Clapeysron on 17/12/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#ifndef Gui_hpp
#define Gui_hpp
#include "opengl_header.h"
#include "Block.hpp"

class Gui {
public:
    ~Gui();
    void gui_init();
    void draw(int screen_width, int screen_height);
    Shader Gui_Shader;
private:
    unsigned int VBO, VAO;
};

#endif /* Gui_hpp */
