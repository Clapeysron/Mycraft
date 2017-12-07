//
//  ChunkData.cpp
//  MineCraft
//
//  Created by apple on 17/11/14.
//  Copyright © 2017年 apple. All rights reserved.
//

#include "ChunkData.hpp"

inline void SubChunk::setPathHistory(int direction){
    pathHistory |= direction;
}

inline int SubChunk::getPathHistory(){
    return pathHistory;
}

//视锥体裁剪
bool SubChunk::inFrustum(int x, int y, int z){
    int chunkCoordinate[8][3] =
    {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1,
        1, 0 ,0, 1, 0, 1, 1, 1, 0, 1, 1, 1};
    int tmpx, tmpy, tmpz;
    for(int i = 0; i < 8; i++)
    {
        tmpx = x+chunkCoordinate[i][0];
        tmpy = y+chunkCoordinate[i][1];
        tmpz = z+chunkCoordinate[i][2];
        for(int j = 0; j < 6; j++)
        {
            if(frustumPlanes[i][0]*tmpx+frustumPlanes[i][1]*tmpy+frustumPlanes[i][2]*tmpz+
               frustumPlanes[i][3] > 0)
                return true;
        }
    }
    return false;
}

inline void SubChunk::updateNeighbor(SubChunk* dir[6]){
    xNeg = dir[0];
    xPos = dir[1];
    zNeg = dir[2];
    zPos = dir[3];
    yNeg = dir[4];
    yPos = dir[5];
}

//更新整个子区块的可见面数据
void SubChunk::updateQuads(){
    char xNegType;
    char xPosType;
    char zNegType;
    char zPosType;
    char yNegType;
    char yPosType;
    
    Quads.clear();
    Quads.reserve(VECTOR_OFFSET); //清空vector
    
    if(isEmpty)
        return; //如果为空，Quads为空
    
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j <16; j++)
        {
            for(int k = 0; k <16; k++)
            {
                if(BlockType[i][j][k] == (char)AIR)
                    continue;
                
                xNegType = (j == 0)? ((xNeg)? xNeg->BlockType[i][15][k]: BOUND) : BlockType[i][j-1][k];
                xPosType = (j == 15)? ((xPos)? xPos->BlockType[i][0][k]: BOUND) : BlockType[i][j+1][k];
                zNegType = (k == 0)? ((zNeg)? zNeg->BlockType[i][j][15]: BOUND) : BlockType[i][j][k-1];
                zPosType = (k == 15)? ((zPos)? zPos->BlockType[i][j][0]: BOUND) : BlockType[i][j][k+1];
                yNegType = (i == 0)? ((yNeg)? yNeg->BlockType[15][j][k]: BOUND) : BlockType[i-1][j][k];
                yPosType = (i == 15)? ((yPos)? yPos->BlockType[0][j][k]: BOUND) : BlockType[i+1][j][k];
                
                if(yPosType & 0x80) //up
                {
                    addVertices(YPOS, i, j, k);
                } //如果相邻方块为透明或未填满，则该面为可见面
                if(yNegType & 0x80) //down
                {
                    addVertices(YNEG, i, j, k);
                }
                if(xPosType & 0x80) //right
                {
                    addVertices(XPOS, i, j, k);
                }
                if(xNegType & 0x80) //left
                {
                    addVertices(XNEG, i, j, k);
                }
                if(zPosType & 0x80) //front
                {
                    addVertices(ZPOS, i, j, k);
                }
                if(zNegType & 0x80) //behind
                {
                    addVertices(ZNEG, i, j, k);
                }
            }
        }
    }
    if(Quads.size() > 0)
        bufferObject.updateBuffer(true, &Quads[0], Quads.size()); //更新VBO绑定数据
}

//添加子区块某个面的可见面(只在边界面变为非边界面时调用)
void SubChunk::updateQuads(int side)
{
    bool dataChange = false;
    if(isEmpty)
        return; //如果为空，Quads为空
    if(side == LEFT)
    {
        SubChunk *neighbor = xNeg;
        for(int i = 0; i < 16; i++)
        {
            for(int j =0; j < 16; j++)
            {
                if(neighbor->BlockType[i][15][j] & 0x80)
                {
                    addVertices(XNEG, i, 0, j);
                    dataChange = true;
                }
            }
        }
        if(dataChange)
            bufferObject.updateBuffer(false, &Quads[0], Quads.size()); //更新VBO绑定数据*/
    }
    else if(side == RIGHT)
    {
        SubChunk *neighbor = xPos;
        for(int i = 0; i < 16; i++)
        {
            for(int j =0; j < 16; j++)
            {
                if(neighbor->BlockType[i][0][j] & 0x80) //left
                {
                    addVertices(XPOS, i, 15, j);
                    dataChange = true;
                }
            }
        }
        if(dataChange)
            bufferObject.updateBuffer(false, &Quads[0], Quads.size()); //更新VBO绑定数据
    }
    else if(side == BEHIND)
    {
        SubChunk *neighbor = zNeg;
        for(int i = 0; i < 16; i++)
        {
            for(int j =0; j < 16; j++)
            {
                if(neighbor->BlockType[i][j][15] & 0x80) //left
                {
                    addVertices(ZNEG, i, j, 0);
                    dataChange = true;
                }
            }
        }
        if(dataChange)
            bufferObject.updateBuffer(false, &Quads[0], Quads.size()); //更新VBO绑定数据
    }
    else if(side == FRONT)
    {
        SubChunk *neighbor = zPos;
        for(int i = 0; i < 16; i++)
        {
            for(int j =0; j < 16; j++)
            {
                if(neighbor->BlockType[i][j][0] & 0x80) //left
                {
                    addVertices(ZPOS, i, j, 15);
                    dataChange = true;
                }
            }
        }
        if(dataChange)
            bufferObject.updateBuffer(false, &Quads[0], Quads.size()); //更新VBO绑定数据
    }
}

//向vector中添加一个面的数据(QUAD_SIZE个float)
void SubChunk::addVertices(int dir, int y, int x, int z)
{
    if(dir < XNEG || dir > YPOS)
        return;
    else{
        if(Quads.size()+QUAD_SIZE >= Quads.capacity())
        {
            Quads.reserve(Quads.size()+VECTOR_OFFSET);
        }
        float tmp[QUAD_SIZE];
        memcpy(tmp, vertex[dir], QUAD_SIZE*sizeof(float));
        for(int m = 0; m < QUAD_SIZE; m = m+VERTEX_SIZE)
        {
            tmp[m] += x+this->x;
            tmp[m+1] += y+this->y;
            tmp[m+2] += z+this->z;
        }
        set_texture(tmp, SubChunk::BlockType[y][x][z], dir);
        Quads.insert(Quads.end(), tmp, tmp+QUAD_SIZE);
    }
}

void SubChunk::set_texture(float* tmp, char type, int dir) {
    if (type == SOIL) {
        for(int m = 0; m < QUAD_SIZE; m = m+VERTEX_SIZE)
        {
            if (dir == YPOS) {
                tmp[m+6] += GRASSLAND_TOP_X;
                tmp[m+7] += GRASSLAND_TOP_Y;
            } else if (dir == YNEG) {
                tmp[m+6] += SOIL_X;
                tmp[m+7] += SOIL_Y;
            } else {
                tmp[m+6] += SOIL_X;
                tmp[m+7] += SOIL_Y;
            }
        }
    } else if (type == ROCK) {
        for(int m = 0; m < QUAD_SIZE; m = m+VERTEX_SIZE) {
            tmp[m+6] += ROCK_X;
            tmp[m+7] += ROCK_Y;
        }
    }
}

//广度优先搜索寻找渲染区块
void SubChunk::adjBlocksEnqueue(){
    
    if((adjVisibility&LEFT) != 0 && xNeg != NULL && xNeg->pathHistory == 0 && (pathHistory & RIGHT) == 0 /*&& inFrustum(x-16, y, z)*/)
    {
        xNeg->pathHistory |= LEFT|pathHistory;
        scanQueue.push(xNeg);
    }//同时满足邻接可见性+非边界+没有入队过+不是在走回头路+通过视锥体裁剪5个条件时，邻居区块入队
    if((adjVisibility&RIGHT) != 0 && xPos != NULL && xPos->pathHistory == 0 && (pathHistory & LEFT) == 0 /*&& inFrustum(x+16, y, z)*/)
    {
        xPos->pathHistory |= RIGHT|pathHistory;
        scanQueue.push(xPos);
    }
    if((adjVisibility&BEHIND) != 0 && zNeg != NULL && zNeg->pathHistory == 0 && (pathHistory & FRONT) == 0 /*&& inFrustum(x, y, z-16)*/)
    {
        zNeg->pathHistory |= BEHIND|pathHistory;
        scanQueue.push(zNeg);
    }
    if((adjVisibility&FRONT) != 0 && zPos != NULL && zPos->pathHistory == 0 && (pathHistory & BEHIND) == 0 /*&& inFrustum(x, y, z+16)*/)
    {
        zPos->pathHistory |= FRONT|pathHistory;
        scanQueue.push(zPos);
    }
    if((adjVisibility&DOWN) != 0 && yNeg != NULL && yNeg->pathHistory == 0 && (pathHistory & UP) == 0 /*&&inFrustum(x, y-16, z)*/)
    {
        yNeg->pathHistory |= DOWN|pathHistory;
        scanQueue.push(yNeg);
    }
    if((adjVisibility&UP) != 0 && yPos != NULL && yPos->pathHistory == 0 && (pathHistory & DOWN) == 0 /*&&
                                                                                                       inFrustum(x, y+16, z)*/)
    {
        yPos->pathHistory |= UP|pathHistory;
        scanQueue.push(yPos);
    }
}

//更新方块的邻接可见性（在新建区块和放置非透明区块时候调用）
void SubChunk::updateVisibility(){
    if(count == 4096)
        adjVisibility = NO_DIR;
    else if(count < 256)
        adjVisibility = ALL_DIR; //非透明方块等于4096邻接可见性均为false，小于256，所有邻接可见性均为true
    else{
        for(int i = 0; i < 16; i++)
        {
            for(int j = 0; j < 16; j++)
            {
                if(BlockType[0][i][j] & 0x80)
                {
                    adjVisibility |= DOWN;
                }
                
                if(BlockType[15][i][j] & 0x80)
                {
                    adjVisibility |= UP;
                }
                
                if(BlockType[i][0][j] & 0x80)
                {
                    adjVisibility |= LEFT;
                }
                
                if(BlockType[i][15][j] & 0x80)
                {
                    adjVisibility |= RIGHT;
                }
                
                if(BlockType[i][j][0] & 0x80)
                {
                    adjVisibility |= BEHIND;
                }
                
                if(BlockType[i][j][15] & 0x80)
                {
                    adjVisibility |= FRONT;
                }
            } //只要边界上有一个半透明或非填满方块，对应面的邻接可见性设置为true
        }
    }
}

inline void SubChunk::setVisibility(int dir){
    adjVisibility |= dir;
}

//移除方块
char SubChunk::removeBlock(int y, int x, int z){
    char type = BlockType[y][x][z];
    BlockType[y][x][z] = AIR;
    if((type&0x80) == 0)
    {
        count--;
        updateQuads();
        if(x == 0 && xNeg != NULL)
        {
            xNeg->addVertices(XPOS, y, 15, z);
            adjVisibility |= LEFT;
            xNeg->bufferObject.updateBuffer(false, &(xNeg->Quads[0]), xNeg->Quads.size()); //更新VBO绑定数据
        }
        if(x == 15 && xPos != NULL)
        {
            xPos->addVertices(XNEG, y, 0, z);
            adjVisibility |= RIGHT;
            xPos->bufferObject.updateBuffer(false, &(xPos->Quads[0]), xPos->Quads.size()); //更新VBO绑定数据
        }
        if(z == 0 && zNeg != NULL)
        {
            zNeg->addVertices(ZPOS, y, x, 15);
            adjVisibility |= BEHIND;
            zNeg->bufferObject.updateBuffer(false, &(zNeg->Quads[0]), zNeg->Quads.size()); //更新VBO绑定数据
        }
        if(z == 15 && zPos != NULL)
        {
            zPos->addVertices(ZNEG, y, x, 0);
            adjVisibility |= FRONT;
            zPos->bufferObject.updateBuffer(false, &(zPos->Quads[0]), zPos->Quads.size()); //更新VBO绑定数据
        }
        if(y == 0 && yNeg != NULL)
        {
            yNeg->addVertices(YPOS, 15, x, z);
            adjVisibility |= DOWN;
            yNeg->bufferObject.updateBuffer(false, &(yNeg->Quads[0]), yNeg->Quads.size()); //更新VBO绑定数据
        }
        if(y == 15 && yPos != NULL)
        {
            yPos->addVertices(YNEG, 0, x, z);
            adjVisibility |= UP;
            yPos->bufferObject.updateBuffer(false, &(yPos->Quads[0]), yPos->Quads.size()); //更新VBO绑定数据
        }
    }//如果方块非透明，更新子区块的可见面。如果处于边界，在相邻子区块中添加一个可见面，且设置该面的邻接可见性为true
    return type;
}

//放置方块(放置或移除方块后，需要更新被改变的子区块的VAO数据)
void SubChunk::placeBlock(char type, int y, int x, int z){
    if(type == (char)AIR)
    {
        return;
    } //如果人物手中什么都没有，不放置方块
    BlockType[y][x][z] = type;
    if((type&0x80) == 0)
    {
        count++;
        updateQuads();
        if(x == 0 || x == 15 || z == 0 || z == 15 || y == 0 || y == 15)
        {
            updateVisibility();
        }
        if(x == 0 && xNeg != NULL)
            xNeg->updateQuads();
        if(x == 15 && xPos != NULL)
            xPos->updateQuads();
        if(z == 0 && zNeg != NULL)
            zNeg->updateQuads();
        if(z == 15 && zPos != NULL)
            zPos->updateQuads();
        if(y == 0 && yNeg != NULL)
            yNeg->updateQuads();
        if(y == 15 && yPos != NULL)
            yPos->updateQuads();
    } //如果放置的是非透明方块，更新子区块的可见面与临接可见性。如果方块处于边界，那么相邻的子区块也要更新可见面
}

//子区块的某个方块被点击时，判断是放置还是移动
void SubChunk::BlockClicked(char type, int y, int x, int z){
    if(BlockType[y][x][z] == (char)AIR)
        placeBlock(type, y, x, z); //如果点击方块为空，放置
    else
    {
        for(int i = 0; i < sizeof(unclickable); i++)
            if(BlockType[y][x][z] == unclickable[i])
                return;
        removeBlock(y, x, z);
    } //如果点击方块非空，移动（部分方块不可移动，如水）
}


float Noise(int x, int y){
    int n = x+y*57;
    n = (n<<13)^n;
    return (1.0-((n*(n*n*15731+789221)+1376312589)&0x7fffffff)/1073741824.0);
}

float SmoothedNoise(int x, int y)
{
    float corners = (Noise(x-1, y-1)+Noise(x+1, y-1)+Noise(x-1, y+1)+Noise(x+1, y+1))/16;
    float sides = (Noise(x-1, y)+Noise(x+1, y)+Noise(x, y-1)+Noise(x, y+1))/8;
    float center = Noise(x, y)/4;
    return corners + sides + center;
}

float Interpolate(float a, float b, float x)
{
    float ft = x*M_PI;
    float f = (1-cos(ft))*0.5;
    return a*(1-f)+b*f;
}


float InterpolatedNoise(float x, float y)
{
    int integerX = int(x);
    float fractionalX = x-integerX;
    int integerY = int(y);
    float fractionalY = y-integerY;
    float v1 = SmoothedNoise(integerX, integerY);
    float v2 = SmoothedNoise(integerX+1, integerY);
    float v3 = SmoothedNoise(integerX, integerY+1);
    float v4 = SmoothedNoise(integerX+1, integerY+1); //若x，y的变化不大，则v1v2v3v4不会发生变化
    float i1 = Interpolate(v1, v2, fractionalX);
    float i2 = Interpolate(v3, v4, fractionalX); //采用cosin插值
    return Interpolate(i1, i2, fractionalY);
}


//persistence越接近1，波动越明显；n代表需要叠加的倍频数
float PerlinNoise(float x, float y, float persistence, int n)
{
    float total = 0;
    x = 0.01*x;
    y = 0.01*y;
    if(persistence<0.0 || persistence>1.0)
    {
        return -1;
    }
    for(int i = 0; i < n; i++)
    {
        float frequency = pow(2, i);
        float amplitude = pow(persistence, i);
        total = total + InterpolatedNoise(x*frequency, y*frequency)*amplitude;//frequence越大，起伏越小
    }
    return total;
}

Chunk::Chunk(int x, int z){
    this->x = x;
    this->z = z;
    xNeg = xPos = zNeg = zPos = NULL;
    for(int i = 0; i < 16; i++)
    {
        subChunks[i] = new SubChunk(i*16, x, z);
    }
    if(!readFile(to_string(x)+"_"+to_string(z)))
        generateMap();  //data not exist, then generate
}

Chunk::~Chunk(){
    for(int i = 0; i < 16; i++)
    {
        delete subChunks[i];
    }
}

//利用二维柏林噪声生成地图
bool Chunk::generateMap(bool isSea, int seaLevel)
{
    int max = 0;
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            height[i][j] = (int)(120+PerlinNoise(x+i, z+j, 0.1, 5)*128);
            if(max < height[i][j])
                max = height[i][j];
        }
    } //the height of soil layer
    maxHeight = max;
    
    
    int heightRock[16][16];
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            int tmpHeight = (int)(115+PerlinNoise(x+i, z+j, 0.1, 5)*128);
            if(tmpHeight >= height[i][j]-SOIL_THICKNESS)
                heightRock[i][j] = height[i][j]-SOIL_THICKNESS;
            else
                heightRock[i][j] = tmpHeight;
        }
    } //the height of rock layer
    
    for(int m = 0; m < 16; m++)
    {
        if(m*16 > maxHeight){
            subChunks[m]->isEmpty = true;
            memset(subChunks[m]->BlockType, 0x80, 16*16*16*sizeof(char));
            continue;
        }
        subChunks[m]->isEmpty = false;
        for(int i = 0; i < 16; i++)
        {
            for(int j = 0; j < 16; j++)
            {
                for(int k = 0; k < 16; k++)
                {
                    int tmpHeight = i+m*16;
                    if(tmpHeight > height[j][k])
                    {
                        subChunks[m]->BlockType[i][j][k] = AIR;
                    }
                    else if(tmpHeight > heightRock[j][k])
                    {
                        subChunks[m]->BlockType[i][j][k] = SOIL;
                        subChunks[m]->count++; //只统计非透明砖块
                    }
                    else
                    {
                        subChunks[m]->BlockType[i][j][k] = ROCK;
                        subChunks[m]->count++;
                    }
                }
            }
        }
    }
    //writeFile(to_string(xPos)+"_"+to_string(zPos));
    return true;
}

//仅用于测试，该模块能正确运行后去掉
char* Chunk::readChunk()
{
    char *ret = (char *)malloc(16*16*256*sizeof(char));
    int size = 16*16*16;
    for(int i = 0; i < 16; i++)
        memcpy(ret+i*size, subChunks[i]->BlockType, size*sizeof(char));
    return ret;
}

bool Chunk::readFile(string filePath){
    fstream fin;
    fin.open(filePath,ios::in);
    if(!fin)
        return false;
    else
    {
        //TODO
        fin.close();
        return true;
    }
}

bool Chunk::writeFile(string filePath){
    fstream fout;
    fout.open(filePath);
    if(!fout)
        return false;
    else
    {
        //TODO
        fout.close();
        return true;
    }
}

inline void Chunk::setCoordinate(int x, int z){
    this->x = x;
    this->z = z;
}

//void ChunkData::draw_chunk(glm::vec3 position, glm::mat4 view, glm::mat4 projection, Shader& Block_Shader, char type) {
//    for(int i = 0; i < 16; i++) {
//
//    }
//}

void Chunk::updateNeighbor(Chunk* xNeg, Chunk* xPos, Chunk* zNeg, Chunk* zPos){
    SubChunk *dir[6];
    
    this->xPos = xPos;
    this->xNeg = xNeg;
    this->zPos = zPos;
    this->zNeg = zNeg;
    
    for(int i = 0; i < 16; i++)
    {
        dir[0] = (xNeg)? xNeg->subChunks[i]: NULL;
        dir[1] = (xPos)? xPos->subChunks[i]: NULL;
        dir[2] = (zNeg)? zNeg->subChunks[i]: NULL;
        dir[3] = (zPos)? zPos->subChunks[i]: NULL;
        dir[4] = (i == 0)? NULL: subChunks[i-1]; //y-
        dir[5] = (i == 15)? NULL: subChunks[i+1]; //y+
        subChunks[i]->updateNeighbor(dir);
    } //更新所有子区块的邻居
}





VisibleChunks::VisibleChunks(float x, float y, float z){
    int ChunkX, ChunkZ;
    int SubChunkIndex;
    
    ChunkX = (x >= 0)?((int)x)/16*16:(((int)x)/16-1)*16;
    ChunkZ = (z >= 0)?((int)z)/16*16:(((int)z)/16-1)*16;
    
    SubChunkIndex = (int)(y/16);
    
    for(int i = 0; i < 2*RADIUS+1; i++)
    {
        for(int j = 0; j <2*RADIUS+1; j++)
        {
            int tmpx = ChunkX+(i-RADIUS)*16;
            int tmpz = ChunkZ+(j-RADIUS)*16;
            Chunks[i][j] = new Chunk(tmpx, tmpz);
        }
    } //初始化所有区块
    curChunk = Chunks[RADIUS][RADIUS];
    curSubChunk = curChunk->subChunks[SubChunkIndex];
    initQuads(); //初始化所有子区块的可见面
}

//初始化所有子区块的可见面
void VisibleChunks::initQuads(){
    initNeighbor(); //初始化所有区块和子区块的邻居
    for(int i = 0; i < 2*RADIUS+1; i++)
    {
        for(int j = 0; j < 2*RADIUS+1; j++)
        {
            for(int k = 0; k < 16; k++)
            {
                Chunks[i][j]->subChunks[k]->updateQuads(); //更新子区块的可见面
                Chunks[i][j]->subChunks[k]->updateVisibility(); //更新子区块的邻接可见性
            }
        }
    }
}

//初始化所有区块和子区块的邻居（没有邻居则为NULL）
void VisibleChunks::initNeighbor(){
    for(int i = 0; i < 2*RADIUS+1; i++)
    {
        for(int j = 0; j < 2*RADIUS+1; j ++)
        {
            Chunk *xNegChunk = (i > 0)? Chunks[i-1][j]: NULL;
            Chunk *xPosChunk = (i < 2*RADIUS)? Chunks[i+1][j]: NULL;
            Chunk *zNegChunk = (j > 0)? Chunks[i][j-1]: NULL;
            Chunk *zPosChunk = (j < 2*RADIUS)? Chunks[i][j+1]: NULL;
            Chunks[i][j]->updateNeighbor(xNegChunk, xPosChunk, zNegChunk, zPosChunk);
        }
    }
}

VisibleChunks::~VisibleChunks(){
    for(int i = 0; i < 2*RADIUS+1; i++)
        for(int j = 0; j < 2*RADIUS+1; j++)
            delete Chunks[i][j];
}

bool VisibleChunks::updataChunks(float y, float x, float z){
    int ChunkX, ChunkZ;
    int SubChunkIndex;
    int dir = NO_DIR;
    
    ChunkX = (x >= 0)?((int)x)/16*16:(((int)x)/16-1)*16;
    ChunkZ = (z >= 0)?((int)z)/16*16:(((int)z)/16-1)*16;
    SubChunkIndex = ((int)y)/16;
    //计算当前人物所在区块的x, y, z坐标
    
    if(ChunkX == curChunk->x && ChunkZ == curChunk->z){
        curSubChunk = curChunk->subChunks[SubChunkIndex]; //更新当前所在子区块
        return false; //如果当前所在区块仍为中心区块，不用更新
    }
    else if(ChunkX < curChunk->x) //to left
    {
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            Chunk *tmp;
            tmp = Chunks[2*RADIUS][i];
            for(int j = 2*RADIUS; j > 0; j--)
            {
                Chunks[j][i] = Chunks[j-1][i];
            }
            
            delete tmp;
            Chunks[0][i] = new Chunk(ChunkX-RADIUS*16, ChunkZ+(i-RADIUS)*16);
        } //向左走时，所有区块向右移动一格
        dir = LEFT;
    }
    else if(ChunkX > curChunk->x) //to right
    {
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            Chunk *tmp;
            tmp = Chunks[0][i];
            for(int j = 0; j < 2*RADIUS; j++)
            {
                Chunks[j][i] = Chunks[j+1][i];
            }
            
            delete tmp;
            Chunks[2*RADIUS][i] = new Chunk(ChunkX+RADIUS*16, ChunkZ+(i-RADIUS)*16);
        }
        dir = RIGHT;
    } //向右走时，所有区块向左移动一格
    else if(ChunkZ > curChunk->z) //to front
    {
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            Chunk *tmp;
            tmp = Chunks[i][0];
            for(int j = 0; j < 2*RADIUS; j++)
            {
                Chunks[i][j] = Chunks[i][j+1];
            }
            
            delete tmp;
            Chunks[i][2*RADIUS] = new Chunk(ChunkX+(i-RADIUS)*16, ChunkZ+RADIUS*16);
        }
        dir = FRONT;
    } //向前走时，所有区块向后移动一格
    else if(ChunkZ < curChunk->z) //to behind
    {
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            Chunk *tmp;
            tmp = Chunks[i][2*RADIUS];
            for(int j = 2*RADIUS; j > 0; j--)
            {
                Chunks[i][j] = Chunks[i][j-1];
            }
            
            delete tmp;
            Chunks[i][0] = new Chunk(ChunkX+(i-RADIUS)*16, ChunkZ-RADIUS*16);
        }
        dir = BEHIND;
    } //向后走时，所有区块向前移动一格
    curChunk = Chunks[RADIUS][RADIUS];
    curSubChunk = curChunk->subChunks[SubChunkIndex]; //更新当前区块和子区块
    if(dir)
        updateQuads(dir); //根据移动方向，更新边界区块的可见面
    return true;
}

void VisibleChunks::updateQuads(int dir){
    updateNeighbor(dir);
    if(dir == LEFT)
    {
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            for(int j = 0; j < 16; j++)
            {
                Chunks[1][i]->subChunks[j]->updateQuads();
                Chunks[0][i]->subChunks[j]->updateQuads();
                Chunks[0][i]->subChunks[j]->updateVisibility();
            }
        } //只更新左边界（可见面和邻接可见性）和左边第二列（添加最左面)的可见面
    }
    else if(dir == RIGHT)
    {
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            for(int j = 0; j < 16; j++)
            {
                Chunks[2*RADIUS-1][i]->subChunks[j]->updateQuads();
                Chunks[2*RADIUS][i]->subChunks[j]->updateQuads();
                Chunks[2*RADIUS][i]->subChunks[j]->updateVisibility();
            }
        } //只更新右边界（可见面和邻接可见性）和右边第二列（添加最右面)的可见面
    }
    else if(dir == BEHIND)
    {
        
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            for(int j = 0; j < 16; j++)
            {
                Chunks[i][1]->subChunks[j]->updateQuads();
                Chunks[i][0]->subChunks[j]->updateQuads();
                Chunks[i][0]->subChunks[j]->updateVisibility();
            }
        }
    } //只更新后边界（可见面和邻接可见性）和后边第二列（添加最后面)的可见面
    else if(dir == FRONT)
    {
        
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            for(int j = 0; j < 16; j++)
            {
                Chunks[i][2*RADIUS-1]->subChunks[j]->updateQuads();
                Chunks[i][2*RADIUS]->subChunks[j]->updateQuads();
                Chunks[i][2*RADIUS]->subChunks[j]->updateVisibility();
            }
        }
    } //只更新前边界（可见面和邻接可见性）和前边第二列（添加最前面)的可见面
}

//在人物从一个区块移动到另一个区块时，更新邻居
void VisibleChunks::updateNeighbor(int dir){
    if(dir == LEFT)
    {
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            Chunks[2*RADIUS][i]->xPos = NULL;
            for(int j = 0; j < 16; j++)
            {
                Chunks[2*RADIUS][i]->subChunks[j]->xPos = NULL;
            } //右边界上的区块邻居都变为NULL
            
            Chunks[1][i]->xNeg = Chunks[0][i];
            for(int j = 0; j < 16; j++)
            {
                Chunks[1][i]->subChunks[j]->xNeg = Chunks[0][i]->subChunks[j];
            } //左边第二列的区块设置为左边界上的区块
            
            Chunk *zNegChunk = (i > 0)? Chunks[0][i-1]: NULL;
            Chunk *zPosChunk = (i < 2*RADIUS)? Chunks[0][i+1]: NULL;
            Chunks[0][i]->updateNeighbor(NULL, Chunks[1][i], zNegChunk, zPosChunk);
            // 新生成区块的邻居整体更新
        }
    }
    else if(dir == RIGHT)
    {
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            Chunks[0][i]->xNeg = NULL;
            for(int j = 0; j < 16; j++)
            {
                Chunks[0][i]->subChunks[j]->xNeg = NULL;
            }
            
            Chunks[2*RADIUS-1][i]->xPos = Chunks[2*RADIUS][i];
            for(int j = 0; j < 16; j++)
            {
                Chunks[2*RADIUS-1][i]->subChunks[j]->xPos = Chunks[2*RADIUS][i]->subChunks[j];
            }
            
            Chunk *zNegChunk = (i > 0)? Chunks[2*RADIUS][i-1]: NULL;
            Chunk *zPosChunk = (i < 2*RADIUS)? Chunks[2*RADIUS][i+1]: NULL;
            Chunks[2*RADIUS][i]->updateNeighbor(Chunks[2*RADIUS-1][i], NULL, zNegChunk, zPosChunk);
        }
    }
    else if(dir == BEHIND)
    {
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            Chunks[i][2*RADIUS]->zPos = NULL;
            for(int j = 0; j < 16; j++)
            {
                Chunks[i][2*RADIUS]->subChunks[j]->zPos = NULL;
            }
            
            Chunks[i][1]->zNeg = Chunks[i][0];
            for(int j = 0; j < 16; j++)
            {
                Chunks[i][1]->subChunks[j]->zNeg = Chunks[i][0]->subChunks[j];
            }
            
            Chunk *xNegChunk = (i > 0)? Chunks[i-1][0]: NULL;
            Chunk *xPosChunk = (i < 2*RADIUS)? Chunks[i+1][0]: NULL;
            Chunks[i][0]->updateNeighbor(xNegChunk, xPosChunk, NULL, Chunks[i][1]);
        }
    }
    else if(dir == FRONT)
    {
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            Chunks[i][0]->zNeg = NULL;
            for(int j = 0; j < 16; j++)
            {
                Chunks[i][0]->subChunks[j]->zNeg = NULL;
            }
            
            Chunks[i][2*RADIUS-1]->zPos = Chunks[i][2*RADIUS];
            for(int j = 0; j < 16; j++)
            {
                Chunks[i][2*RADIUS-1]->subChunks[j]->zPos = Chunks[i][2*RADIUS]->subChunks[j];
            }
            
            Chunk *xNegChunk = (i > 0)? Chunks[i-1][2*RADIUS]: NULL;
            Chunk *xPosChunk = (i < 2*RADIUS)? Chunks[i+1][2*RADIUS]: NULL;
            Chunks[i][2*RADIUS]->updateNeighbor(xNegChunk, xPosChunk, Chunks[i][2*RADIUS-1], NULL);
        }
    }
}


inline Chunk *VisibleChunks::getCurChunk(){
    return curChunk;
}


inline SubChunk *VisibleChunks::getCurSubChunk(){
    return curSubChunk;
}



void VisibleChunks::getRenderingSubChunks(int y, int x, int z){
    SubChunk* tmp;
    
    //clear
    while(!renderQueue.empty()) //清空上一帧的渲染区块队列
        renderQueue.pop();
    clearPathHistory(); //清空上一帧的广搜路径
    
    //scanQueue.push(curSubChunk); //当前所在区块入队伍
    if(floodFill(y, x, z))
    {
        renderQueue.push(curSubChunk);
        curSubChunk->setPathHistory(ALL_DIR);
        if(curSubChunk->xNeg != NULL){
            scanQueue.push(curSubChunk->xNeg);
            curSubChunk->xNeg->pathHistory |= LEFT;
        }
        if(curSubChunk->xPos != NULL){
            scanQueue.push(curSubChunk->xPos);
            curSubChunk->xPos->pathHistory |= RIGHT;
        }
        if(curSubChunk->zNeg != NULL){
            scanQueue.push(curSubChunk->zNeg);
            curSubChunk->zNeg->pathHistory |= BEHIND;
        }
        if(curSubChunk->zPos != NULL){
            scanQueue.push(curSubChunk->zPos);
            curSubChunk->zPos->pathHistory |= FRONT;
        }
        if(curSubChunk->yNeg != NULL){
            scanQueue.push(curSubChunk->yNeg);
            curSubChunk->yNeg->pathHistory |= DOWN;
        }
        if(curSubChunk->yPos != NULL){
            scanQueue.push(curSubChunk->yPos);
            curSubChunk->yPos->pathHistory |= UP;
        }
        while(!scanQueue.empty())
        {
            //cout<<scanQueue.size()<<endl;
            tmp = scanQueue.front();
            tmp->adjBlocksEnqueue();
            if(tmp->Quads.size())
                renderQueue.push(tmp);
            scanQueue.pop();
        }
    } //洪水填充，区块非封闭且非面壁时，周围六个方块无条件入队。
    else{
        renderQueue.push(curSubChunk);
    } //只渲染当前区块
    //    return renderQueue;
}

//洪水填充，判断是否处在一个封闭的自区块中 TODO: 判断是否处于面壁状态
bool VisibleChunks::floodFill(int y, int x, int z){
    class Coordinate{
    public:
        int x;
        int y;
        int z;
        Coordinate(int y, int x, int z){ this->x = x; this->y = y; this->z = z;}
    };
    stack<Coordinate> s;
    Coordinate *tmp;
    const int offsety[6] = {1, -1, 0, 0, 0, 0};
    const int offsetx[6] = {0, 0, 1, -1, 0, 0};
    const int offsetz[6] = {0, 0, 0, 0, 1, -1};
    int tmpx, tmpy, tmpz;
    
    
    x = x%16;
    if(x < 0)
        x += 16;
    
    y = y%16;//r如果y<0, 需要抛出异常
    
    z = z%16;
    if(z < 0)
        z += 16;
    
    int visited[16][16][16] = {0};
    visited[y][x][z] = 1;
    s.push(Coordinate(y, x, z));
    
    while(!s.empty())
    {
        tmp = &s.top();
        if(tmp->x == 0 || tmp->x == 15 || tmp->y == 0 || tmp->y == 15 ||
           tmp->z == 0 || tmp->z == 15)
        {
            return true;
        }
        else
        {
            int x = tmp->x;
            int y = tmp->y;
            int z = tmp->z;
            for(int i = 0; i < 6; i++)
            {
                tmpx = x+offsetx[i];
                tmpy = y+offsety[i];
                tmpz = z+offsetz[i];
                if((curSubChunk->BlockType[tmpy][tmpx][tmpz] & 0x80) != 0 &&
                   !visited[tmpy][tmpx][tmpz])
                {
                    s.push(Coordinate(tmpy, tmpx, tmpz));
                    visited[tmpy][tmpx][tmpz] = 1;
                }
            }
        }
        s.pop();
    }
    return false;
}

//每帧寻找需要渲染的物体前，清空上一帧留下的广度搜索路径
void VisibleChunks::clearPathHistory(){
    for(int i = 0; i < 2*RADIUS+1; i++)
    {
        for(int j = 0; j < 2*RADIUS+1; j++)
        {
            for(int k = 0; k < 16; k++)
            {
                Chunks[i][j]->subChunks[k]->pathHistory = NO_DIR;
            }
        }
    }
}

//渲染物体
void VisibleChunks::draw(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection, Shader& Block_Shader){
    calcFrustumPlane(view, projection);
    cout<<"y:"<<(int)cameraPos.y<<" x:"<<(int)cameraPos.x<<" z:"<<(int)cameraPos.z<<endl;
    /*if(flag == 0){
     //getRenderingSubChunks((int)cameraPos.y, (int)cameraPos.x, (int)cameraPos.z);//float为负数时候怎么rounding？？？？
     flag = 1;
     }*/
    updataChunks((int)cameraPos.y, (int)cameraPos.x, (int)cameraPos.z);
    getRenderingSubChunks((int)cameraPos.y, (int)cameraPos.x, (int)cameraPos.z);//float为负数时候怎么rounding？？？？
    SubChunk *tmp;
    Block_Shader.use();
    glm::mat4 model(1);
    //model = glm::translate(model, position);
    Block_Shader.setMat4("view", view);
    Block_Shader.setMat4("projection", projection);
    Block_Shader.setMat4("model", model);
    cout<<renderQueue.size()<<endl;
    for(int i = 0; i < renderQueue.size(); i++)
    {
        tmp = renderQueue.front();
        glBindVertexArray(tmp->bufferObject.getVAO());
        glDrawArrays(GL_TRIANGLES, 0, (int)(tmp->Quads.size()/VERTEX_SIZE));
        renderQueue.pop();
        renderQueue.push(tmp);
    }
}

//计算视锥体各个面的方程
void VisibleChunks::calcFrustumPlane(glm::mat4 view, glm::mat4 projection){
    glm::mat4 matrix = projection*view;
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            frustumPlanes[i][j] = matrix[3][j]+glm::pow(-1, i)*matrix[i/2][j];
        }
    }//left, right, bottom, top, near, far
}




