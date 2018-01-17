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
#include <set>
#include <map>
#include "Block.hpp"
#include "BlockInfo.hpp"
#include "opengl_header.h"
#include "game_settings.h"

#define PERSISTENCE 0.1f  //(0.0f, 1.0f)
#define OCTAVES 5

class Render;

using namespace std;

class SubChunk;
static queue<SubChunk*> scanQueue;

extern float vertex[FACE_TYPE_NUM][QUAD_SIZE];
extern float alternative[FACE_TYPE_NUM][QUAD_SIZE];
extern float vertices[6][48];
extern float waterVertices[QUAD_SIZE];

static int unclickable[] = {WATER};

static float frustumPlanes[6][4];

class TransQuad{
public:
    TransQuad(int y, int x, int z, int dir) {
        //this->pos = glm::vec3(x, y, z);//中心??
        //this->dir = dir;
        memcpy(vertices, vertex[dir], QUAD_SIZE*sizeof(float));
        for(int m = 0; m < QUAD_SIZE; m = m+VERTEX_SIZE)
        {
            vertices[m] += x;
            vertices[m+1] += y;
            vertices[m+2] += z;
            vertices[m+6] += WATER_X;
            vertices[m+7] += WATER_Y;
        }
        //set_texture(tmp, SubChunk::BlockType[y][x][z], dir);
    }
    //glm::vec3 pos;
    //int dir;
    float vertices[QUAD_SIZE];
};
//static vector<float> transQuads;
class SubChunk;
class Chunk;

class LuminousObj {
    friend class SubChunk;
public:
    LuminousObj(int y, int x, int z, char type, SubChunk *subChunk) {
        this->y = y;
        this->x = x;
        this->z = z;
        this->type = type;
        this->subChunk = subChunk;
    }
private:
    char type;
    int y;
    int x;
    int z;
    SubChunk *subChunk;
    set<SubChunk *> inflenced;
};

class SubChunk {
    friend class Chunk;
    friend class VisibleChunks;
public:
    SubChunk(int y, int x, int z){
        //memset(BlockType, 0, sizeof(BlockType));
        isEmpty = true;
        count = 0;
        pathHistory = 0;
        adjVisibility = ALL_DIR;
        xNeg = xPos = zNeg = zPos = yNeg = yPos = NULL;
        QuadSize = 0;
        this->y = y;
        this->x = x;
        this->z = z;
        //初始化阴影
        memset(vertexShadow, 0, 16*16*16*sizeof(unsigned short));
        //初始化光照
        memset(brightness, 0, 16*16*16*sizeof(char));
    }
    SubChunk* recycle(int y, int x, int z);
    void setPathHistory(int direction); //clear path history
    int getPathHistory(); //????????
    Block *getBufferObject();
    void updateCount();
    void updateNeighbor(Chunk* parent, SubChunk* dir[6]); //for walking update
    void updateVisibility(); //for walking update
    void setVisibility(int dir); //for remove&place update
    void updateQuads(); //for walking update
    void updateQuads(int side); //for walkong update
    void adjBlocksEnqueue(); //for rendering
    void addVertices(int dir, int y, int x, int z, vector<float> *Quads); //called by updateQuads
    void addVertexShadow(int y, int x, int z);
    //只有移动或者放置非透明块时有影响
    char removeBlock(int y, int x, int z);
    //设置为空气
    //如果在边界，邻接可见性置为true
    //直接update本块，邻接块只添加原来被遮挡的面
    bool placeBlock(char type, int dir, int y, int x, int z);
    //设置type
    //如果在边界，重新判断邻接可见性
    //直接update本块和所有邻接块
    bool waterFilling(int y, int x, int z);
    void BlockClicked(char type, int y, int x, int z);
    void calcBrightness(int y, int x, int z);
    void updateBrightness();
    LuminousObj* removeLuminousObj(int y, int x, int z);
    LuminousObj* eraseLuminousObjs(int y, int x, int z, SubChunk *s);
    LuminousObj* findLuminousObjs(int y, int x, int z, SubChunk *s);
private:
    int x;
    int y;
    int z;
    char BlockType[16][16][16]; //yxz, 256-type of cubes
    unsigned short vertexShadow[16][16][16];
    char brightness[16][16][16];
    int count;
    bool isEmpty;
    int pathHistory;
    int adjVisibility;
    long QuadSize;
    //neighbor subchunks
    Chunk* parent;
    SubChunk *xNeg;
    SubChunk *xPos;
    SubChunk *zNeg;
    SubChunk *zPos;
    SubChunk *yNeg;
    SubChunk *yPos;
    //Quads need rendering
    set<LuminousObj *> luminousObjs;
    Block bufferObject;
    bool inFrustum(int x, int y, int z);
    void set_texture(float* tmp, char type, int dir);
};


class Chunk {
    friend class VisibleChunks;
public:
    Chunk(int x, int z);
    ~Chunk();
    Chunk*recycle(int x, int z);
    bool generateMap(); //called by initChunks
    void generateHerb();
    void generateCloud();
    bool generateTree(int y, int x, int z);
    bool readFile(string filePath); //TO-DO, called by initChunks or updateChunks
    bool writeFile(string filePath); //TO-DO, called by updateChunks
    char* readChunk(); //called by render(test ver)
    void addVertices(int dir, int y, int x, int z);
    void addTransBlock(char type, int y, int x, int z);
    void addWater(int y, int x, int z);
    void updateWater();
    void updateTransQuads();
    
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
    vector<float> Water;
    Block bufferObject;
    //vector<float> transQuads;
    //Block transBufObj;
    void updateNeighbor(Chunk *xNegChunk, Chunk *xPosChunk, Chunk *zNegChunk, Chunk *zPosChunk);
    void setCoordinate(int x, int z);
};

class VisibleChunks {
public:
    VisibleChunks(float x, float y, float z);
    ~VisibleChunks();
    void initBlockInfo();
    void initChunks(); //init map
    bool updataChunks(float x, float y, float z); //update map
    Chunk *getCurChunk();
    SubChunk *getCurSubChunk();
    Block HoldBlock;
    queue<SubChunk*> getRenderQueue();
    void getRenderingSubChunks(int y, int x, int z); //called by render
    void draw(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection, Shader& Block_Shader, unsigned int texture_pic, unsigned int depthMap_pic, unsigned int skybox, glm::mat4 lightSpaceMatrix, glm::vec3 lightDirection, glm::vec3 chosen_block_pos, float broken_scale, glm::vec3 Sun_Moon_light, glm::vec3 Ambient_light, bool isDaylight, float dayTime, float starIntensity, bool eye_in_water);
    void drawDepth(Shader& Depth_Shader, unsigned int texture_pic);
    char getBlockType(int y, int x, int z);
    bool placeBlock(glm::vec3 cameraPos, glm::vec3 cameraFront, char type);
    char removeBlock(glm::vec3 cameraPos, glm::vec3 cameraFront);
    glm::vec3 accessibleBlock(glm::vec3 cameraPos, glm::vec3 cameraFront);
    static void addTransQuads(int dir, int x, int y, int z);
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
    void drawTransQuads(glm::vec3 cameraPos, Shader& Block_Shader);
    void drawNormQuads(glm::vec3 cameraPos, Shader& Block_Shader);
};
#endif /* ChunkData_hpp */
