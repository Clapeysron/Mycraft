//
//  Sun_Moon.hpp
//  Mycraft
//
//  Created by Clapeysron on 26/12/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#ifndef Sun_Moon_hpp
#define Sun_Moon_hpp
#include "opengl_header.h"

class Sun_Moon {
public:
    ~Sun_Moon();
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model);
    Shader Sun_Moon_Shader;
    void Sun_Moon_init();
private:
    unsigned int VBO, VAO;
    unsigned int Sun_Moon_Shader_pic;
};

#endif /* Sun_Moon_hpp */
