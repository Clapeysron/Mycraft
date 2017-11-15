//
//  ChunkData.hpp
//  MineCraft
//
//  Created by apple on 17/11/14.
//  Copyright © 2017年 apple. All rights reserved.
//

#ifndef ChunkData_hpp
#define ChunkData_hpp

#include <iostream>
#include <math.h>
#include <cstring>
#include <string>

#define AIR 0
#define ROCK 1
#define SOIL 2
#define GRASS 3
#define WATER 4
#define TRUNK 5
#define LEAF 6

using namespace std;

class SubChunk{
    friend class ChunkData;
    public:
    SubChunk(){
        memset(BlockData, 0, sizeof(BlockData));
        isEmpty = true;
    }
    private:
    char BlockData[16][16][16]; //yxz, 256-type of cubes
    bool isEmpty;
};

class ChunkData {
public:
    ChunkData(int x, int y): xPos(x), yPos(y){}
    bool generateMap(bool isSea = false, int seaLevel = 0);
    bool readFile(string filePath);
    bool writeFile(string filePath);
    char* readChunk();
private:
    SubChunk subChunks[16];
    char height[16][16];
    char maxHeight;
    int xPos; //x-coordinate
    int yPos; //y-coordinate

};



#endif /* ChunkData_hpp */
