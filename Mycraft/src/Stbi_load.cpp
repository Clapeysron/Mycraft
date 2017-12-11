//
//  Stbi_load.cpp
//  Mycraft
//
//  Created by Clapeysron on 08/12/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "Stbi_load.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


unsigned char *stbi_load_out(char const *filename, int *x, int *y, int *comp, int req_comp) {
    return stbi_load(filename, x, y, comp, req_comp);
}

void stbi_image_free_out(void *retval_from_stbi_load) {
    stbi_image_free(retval_from_stbi_load);
}
