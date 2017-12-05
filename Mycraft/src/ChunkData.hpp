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
#include <fstream>
#include <math.h>
#include <cstring>
#include <string>
#include <queue>
#include <vector>
#include <stack>
#include "Block.hpp"
#include "opengl_header.h"

class Render;

//transparent material or model (MSB = 1)
#define AIR 0x80
#define WATER 0x81
#define LEAF 0x82

//non-transparent material (MSB = 0)
#define BOUND 0x00
#define ROCK 0x01
#define SOIL 0x02
#define GRASSLAND 0x03
#define TRUNK 0x04

//for hisrotyPath & adjVisibility
#define UP 0x20
#define DOWN 0x10
#define FRONT 0x08
#define BEHIND 0x04
#define LEFT 0x02
#define RIGHT 0x01
#define ALL_DIR 0x3f
#define NO_DIR 0x00

#define SOIL_THICKNESS 5

//index of vertices array
#define XNEG 0x00
#define XPOS 0x01
#define ZNEG 0x02
#define ZPOS 0x03
#define YNEG 0x04
#define YPOS 0x05

//vector reverse size
#define VECTOR_OFFSET 24576
#define QUAD_SIZE 48

//visible region radius (TO DO: 16)
#define RADIUS 3

using namespace std;

class SubChunk;
static queue<SubChunk*> scanQueue;

extern float vertex[6][48];
extern float vertices[6][48];

static int unclickable[] = {WATER};

static float frustumPlanes[6][4];



class SubChunk{
    friend class Chunk;
    friend class VisibleChunks;
public:
    SubChunk(int y, int x, int z){
        memset(BlockType, 0, sizeof(BlockType));
        isEmpty = true;
        count = 0;
        pathHistory = 0;
        adjVisibility = 0;
        xNeg = xPos = zNeg = zPos = yNeg = yPos = NULL;
        this->y = y;
        this->x = x;
        this->z = z;
    }
    void setPathHistory(int direction); //clear path history
    int getPathHistory(); //????????
    void updateNeighbor(SubChunk* dir[6]); //for walking update
    void updateVisibility(); //for walking update
    void setVisibility(int dir); //for remove&place update
    void updateQuads(); //for walking update
    void updateQuads(int side); //for walkong update
    void adjBlocksEnqueue(); //for rendering
    void addVertices(int dir, int y, int x, int z); //called by updateQuads
    
    //只有移动或者放置非透明块时有影响
    char removeBlock(int y, int x, int z); //TO-DO
    //设置为空气
    //如果在边界，邻接可见性置为true
    //直接update本块，邻接块只添加原来被遮挡的面
    void placeBlock(char type, int y, int x, int z); //TO-DO
    //设置type
    //如果在边界，重新判断邻接可见性
    //直接update本块和所有邻接块
    void BlockClicked(char type, int y, int x, int z);
    
    
private:
    int x;
    int y;
    int z;
    char BlockType[16][16][16]; //yxz, 256-type of cubes
    int count;
    bool isEmpty;
    int pathHistory;
    int adjVisibility;
    //neighbor subchunks
    SubChunk *xNeg;
    SubChunk *xPos;
    SubChunk *zNeg;
    SubChunk *zPos;
    SubChunk *yNeg;
    SubChunk *yPos;
    //Quads need rendering
    vector<float> Quads;
    Block bufferObject;
    
    bool inFrustum(int x, int y, int z);
};


class Chunk {
    friend class VisibleChunks;
public:
    Chunk(int x, int z);
    ~Chunk();
    bool generateMap(bool isSea = false, int seaLevel = 0); //called by initChunks
    bool readFile(string filePath); //TO-DO, called by initChunks or updateChunks
    bool writeFile(string filePath); //TO-DO, called by updateChunks
    char* readChunk(); //called by render(test ver)
    
private:
    SubChunk *subChunks[16];
    int height[16][16];
    int maxHeight;
    int x; //x-coordinate
    int z; //y-coordinate
    //neighbor chunks
    Chunk *xNeg;
    Chunk *xPos;
    Chunk *zNeg;
    Chunk *zPos;
    //called by updateChunks
    void updateNeighbor(Chunk *xNegChunk, Chunk *xPosChunk, Chunk *zNegChunk, Chunk *zPosChunk);
    void setCoordinate(int x, int z);
    

};


class VisibleChunks{
public:
    VisibleChunks(float x, float y, float z);
    ~VisibleChunks();
    void initChunks(); //init map
    bool updataChunks(float x, float y, float z); //update map
    Chunk *getCurChunk();
    SubChunk *getCurSubChunk();
    void getRenderingSubChunks(int y, int x, int z); //called by render
    void draw(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection, Shader& Block_Shader);
    
    
private:
    Chunk *curChunk;
    SubChunk *curSubChunk;
    Chunk *Chunks[2*RADIUS+1][2*RADIUS+1];
    queue<SubChunk*> renderQueue;
    
    void initQuads(); //called by constructor
    void initNeighbor(); //called by initQuads
    void updateQuads(int dir); //called by updataChunks
    void updateNeighbor(int dir); //called by updateQuads
    void clearPathHistory(); //called by getRenderingSubChunks
    bool floodFill(int y, int x, int z); //called by getRenderingSubChunks，TO-DO：face-wall culling
    void calcFrustumPlane(glm::mat4 view, glm::mat4 projection);
};



#endif /* ChunkData_hpp */
