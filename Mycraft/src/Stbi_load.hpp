//
//  Stbi_load.hpp
//  Mycraft
//
//  Created by Clapeysron on 08/12/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#ifndef Stbi_load_hpp
#define Stbi_load_hpp

unsigned char *stbi_load_out(char const *filename, int *x, int *y, int *comp, int req_comp);
void stbi_image_free_out(void *retval_from_stbi_load);
#endif /* Stbi_load_hpp */
