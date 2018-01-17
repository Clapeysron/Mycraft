//
//  ChunkData.cpp
//  MineCraft
//
//  Created by apple on 17/11/14.
//  Copyright © 2017年 apple. All rights reserved.
//

#include "ChunkData.hpp"

inline void SubChunk::setPathHistory(int direction) {
    pathHistory |= direction;
}

inline int SubChunk::getPathHistory() {
    return pathHistory;
}

Block *SubChunk::getBufferObject(){
    return &bufferObject;
}

inline SubChunk* SubChunk::recycle(int y, int x, int z){
    isEmpty = true;
    count = 0;
    pathHistory = 0;
    adjVisibility = 0;
    xNeg = xPos = zNeg = zPos = yNeg = yPos = NULL;
    this->y = y;
    this->x = x;
    this->z = z;
    return this;
}

void SubChunk::updateCount(){
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j ++) {
            for(int k = 0; k < 16; k++) {
                if((BlockType[i][j][k]|0x80) == 0) {
                    count++;
                }
                else if(BlockType[i][j][k] != (char)AIR) {
                    isEmpty = false;
                }
            }
        }
    }
}

//视锥体裁剪
bool SubChunk::inFrustum(int x, int y, int z) {
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

inline void SubChunk::updateNeighbor(Chunk* parent, SubChunk* dir[6]){
    this->parent = parent;
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
    vector<float> Quads;
    //Water.clear();
    //Water.shrink_to_fit();
    
    //Quads.reserve(VECTOR_OFFSET); //清空vector
    
    if(isEmpty)
        return; //如果为空，Quads为空
    
    if(this->y > 150) {
        int m = 0;
        m++;
    }
    
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j <16; j++)
        {
            for(int k = 0; k <16; k++)
            {
                if(BlockType[i][j][k] == (char)AIR)
                    continue;
                if(i == 7 && j == 6 && k == 7) {
                    int m = 0;
                    m++;
                }
                //添加可见面（把电影阴影数据一同传入）
                if((BlockType[i][j][k]&0xc0) != 0xc0) {
                    xNegType = (j == 0)?
                    ((xNeg)?xNeg->BlockType[i][15][k]: BOUND) : BlockType[i][j-1][k];
                    xPosType = (j == 15)?
                    ((xPos)?xPos->BlockType[i][0][k]: BOUND) : BlockType[i][j+1][k];
                    zNegType = (k == 0)?
                    ((zNeg)?zNeg->BlockType[i][j][15]: BOUND) : BlockType[i][j][k-1];
                    zPosType = (k == 15)?
                    ((zPos)?zPos->BlockType[i][j][0]: BOUND) : BlockType[i][j][k+1];
                    yNegType = (i == 0)?
                    ((yNeg)?yNeg->BlockType[15][j][k]: BOUND) : BlockType[i-1][j][k];
                    yPosType = (i == 15)?
                    ((yPos)?yPos->BlockType[0][j][k]: BOUND) : BlockType[i+1][j][k];
                    if(yPosType & 0x80) //up
                    {
                        //先检查顶点阴影(边界不检查阴影)
                        if(i == 15 && yPos->vertexShadow[0][j][k] == 0) {
                            yPos->addVertexShadow(0, j, k);
                        }
                        else if(i < 15 && vertexShadow[i+1][j][k] == 0) {
                            addVertexShadow(i+1, j, k);
                        }
                        addVertices(YPOS, i, j, k, &Quads);
                    } //如果相邻方块为透明或未填满，则该面为可见面
                    if(yNegType & 0x80) //down
                    {
                        if(i == 0 && yNeg->vertexShadow[15][j][k] == 0) {
                            yNeg->addVertexShadow(15, j, k);
                        }
                        else if(i > 0 && vertexShadow[i-1][j][k] == 0) {
                            addVertexShadow(i-1, j, k);
                        }
                        addVertices(YNEG, i, j, k, &Quads);
                    }
                    if(xPosType & 0x80) //right
                    {
                        if(j == 15 && xPos->vertexShadow[i][0][k] == 0) {
                            xPos->addVertexShadow(i, 0, k);
                        }
                        else if(j < 15 && vertexShadow[i][j+1][k] == 0) {
                            addVertexShadow(i, j+1, k);
                        }
                        addVertices(XPOS, i, j, k, &Quads);
                    }
                    if(xNegType & 0x80) //left
                    {
                        if(j == 0 && xNeg->vertexShadow[i][15][k] == 0) {
                            xNeg->addVertexShadow(i, 15, k);
                        }
                        else if(j > 0 && vertexShadow[i][j-1][k] == 0) {
                            addVertexShadow(i, j-1, k);
                        }
                        addVertices(XNEG, i, j, k, &Quads);
                    }
                    if(zPosType & 0x80) //front
                    {
                        if(k == 15 && zPos->vertexShadow[i][j][0] == 0) {
                            zPos->addVertexShadow(i, j, 0);
                        }
                        else if(k < 15 && vertexShadow[i][j][k+1] == 0) {
                            addVertexShadow(i, j, k+1);
                        }
                        addVertices(ZPOS, i, j, k, &Quads);
                    }
                    if(zNegType & 0x80) //behind
                    {
                        if(k == 0 && zNeg->vertexShadow[i][j][15] == 0) {
                            zNeg->addVertexShadow(i, j, 15);
                        }
                        else if(k > 0 && vertexShadow[i][j][k-1] == 0) {
                            addVertexShadow(i, j, k-1);
                        }
                        addVertices(ZNEG, i, j, k, &Quads);
                    }
                }
                else if((BlockType[i][j][k]&0xf0) != 0xf0) {
                    addVertices(XCENTER, i, j, k, &Quads);
                    addVertices(ZCENTER, i, j, k, &Quads);
                }
            }
        }
    }
    if(Quads.size() > 0) {
        QuadSize = Quads.size();
        bufferObject.updateBuffer(true, &Quads[0], Quads.size()); //更新VBO绑定数据
    }
}

void SubChunk::addVertexShadow(int y, int x, int z) {
    char xNegType = (x == 0)? ((xNeg)?xNeg->BlockType[y][15][z]: BOUND) : BlockType[y][x-1][z];
    char xPosType = (x == 15)? ((xPos)?xPos->BlockType[y][0][z]: BOUND) : BlockType[y][x+1][z];
    char zNegType = (z == 0)? ((zNeg)?zNeg->BlockType[y][x][15]: BOUND) : BlockType[y][x][z-1];
    char zPosType = (z == 15)? ((zPos)?zPos->BlockType[y][x][0]: BOUND) : BlockType[y][x][z+1];
    char yNegType = (y == 0)? ((yNeg)?yNeg->BlockType[15][x][z]: BOUND) : BlockType[y-1][x][z];
    char yPosType = (y == 15)? ((yPos)?yPos->BlockType[0][x][z]: BOUND) : BlockType[y+1][x][z];
    //添加点阴影数据
    unsigned short yNeg = ((yNegType&0x80) == 0 && yNegType != BOUND);
    unsigned short yPos = ((yPosType&0x80) == 0 && yPosType != BOUND);
    unsigned short xNeg = ((xNegType&0x80) == 0 && xNegType != BOUND);
    unsigned short xPos = ((xPosType&0x80) == 0 && xPosType != BOUND);
    unsigned short zNeg = ((zNegType&0x80) == 0 && zNegType != BOUND);
    unsigned short zPos = ((zPosType&0x80) == 0 && zPosType != BOUND);
    
    unsigned short tmp;

    tmp = 0;
    tmp += yPos+xPos+zNeg;
    if(tmp == 0)
        tmp++;
    vertexShadow[y][x][z] |= tmp<<VERTEX1_OFFSET;
    
    tmp = 0;
    tmp += yPos+xNeg+zNeg;
    if(tmp == 0)
        tmp++;
    vertexShadow[y][x][z] |= tmp<<VERTEX2_OFFSET;
    
    tmp = 0;
    tmp += yPos+xNeg+zPos;
    if(tmp == 0)
        tmp++;
    vertexShadow[y][x][z] |= tmp<<VERTEX3_OFFSET;
    
    tmp = 0;
    tmp += yPos+xPos+zPos;
    if(tmp == 0)
        tmp++;
    vertexShadow[y][x][z] |= tmp<<VERTEX4_OFFSET;
    
    tmp = 0;
    tmp += yNeg+xPos+zNeg;
    if(tmp == 0)
        tmp++;
    vertexShadow[y][x][z] |= tmp<<VERTEX5_OFFSET;
    
    tmp = 0;
    tmp += yNeg+xNeg+zNeg;
    if(tmp == 0)
        tmp++;
    vertexShadow[y][x][z] |= tmp<<VERTEX6_OFFSET;
    
    tmp = 0;
    tmp += yNeg+xNeg+zPos;
    if(tmp == 0)
        tmp++;
    vertexShadow[y][x][z] |= tmp<<VERTEX7_OFFSET;
    
    tmp = 0;
    tmp += yNeg+xPos+zPos;
    if(tmp == 0)
        tmp++;
    vertexShadow[y][x][z] |= tmp<<VERTEX8_OFFSET;
}

//添加子区块某个面的可见面(只在边界面变为非边界面时调用)
/*void SubChunk::updateQuads(int side)
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
            bufferObject.updateBuffer(false, &Quads[0], Quads.size()); //更新VBO绑定数据
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
}*/

int faceShadow[6][6] = {
    VERTEX5_OFFSET, VERTEX8_OFFSET, VERTEX4_OFFSET,
    VERTEX1_OFFSET, VERTEX5_OFFSET, VERTEX4_OFFSET,
    
    VERTEX6_OFFSET, VERTEX7_OFFSET, VERTEX3_OFFSET,
    VERTEX2_OFFSET, VERTEX6_OFFSET, VERTEX3_OFFSET,
    
    VERTEX7_OFFSET, VERTEX3_OFFSET, VERTEX4_OFFSET,
    VERTEX8_OFFSET, VERTEX7_OFFSET, VERTEX4_OFFSET,
    
    VERTEX6_OFFSET, VERTEX2_OFFSET, VERTEX1_OFFSET,
    VERTEX5_OFFSET, VERTEX6_OFFSET, VERTEX1_OFFSET,
    
    VERTEX2_OFFSET, VERTEX1_OFFSET, VERTEX4_OFFSET,
    VERTEX3_OFFSET, VERTEX2_OFFSET, VERTEX4_OFFSET,
    
    VERTEX6_OFFSET, VERTEX5_OFFSET, VERTEX8_OFFSET,
    VERTEX7_OFFSET, VERTEX6_OFFSET, VERTEX8_OFFSET
};



//向vector中添加一个面的数据(QUAD_SIZE个float)
void SubChunk::addVertices(int dir, int y, int x, int z, vector<float> *Quads)
{
    if(dir < XNEG || dir > ZCENTER)
        return;
    else{
        /*if(Quads.size()+QUAD_SIZE >= Quads.capacity())
        {
            Quads.reserve(Quads.size()+VECTOR_OFFSET);
        }*/
        int absy = this->y+y;
        int absx = this->x+x;
        int absz = this->z+z;
        float tmp[QUAD_SIZE];
        memcpy(tmp, vertex[dir], QUAD_SIZE*sizeof(float));
        
        //设置阴影信息
        int offset[6][3] = {0, -1, 0, 0, 1, 0, 0, 0, -1, 0, 0, 1, -1, 0, 0, 1, 0, 0};
        if(((BlockType[y][x][z]&0x80) == 0 && BlockType[y][x][z] != CLOUD) || BlockType[y][x][z] == (char)LEAF) {
            int tmpy = y+offset[dir][0];
            int tmpx = x+offset[dir][1];
            int tmpz = z+offset[dir][2];
            SubChunk *tmpChunk = this;
            if(tmpy == 16) {
                tmpChunk = yPos;
            }
            else if(tmpy == -1) {
                tmpChunk = yNeg;
                tmpy += 16;
            }
            else if(tmpx == 16) {
                tmpChunk = xPos;
            }
            else if(tmpx == -1) {
                tmpChunk = xNeg;
                tmpx += 16;
            }
            else if(tmpz == 16) {
                tmpChunk = zPos;
            }
            else if(tmpz == -1) {
                tmpChunk = zNeg;
                tmpz += 16;
            }
            if(tmpChunk) {
                unsigned short shadow = tmpChunk->vertexShadow[tmpy%16][tmpx%16][tmpz%16];
                float vertices[4];
                for(int i = 0; i < 4; i++) {
                    unsigned short tmpShadow = (shadow>>faceShadow[dir][i])&0x0003;
                    if(tmpShadow == 2) {
                        tmpy = y-1+2*(int)tmp[VERTEX_SIZE*i+1];
                        tmpx = x-1+2*(int)tmp[VERTEX_SIZE*i];
                        tmpz = z-1+2*(int)tmp[VERTEX_SIZE*i+2];
                        tmpChunk = this;
                        if(tmpy == 16) {
                            tmpChunk = tmpChunk->yPos;
                            tmpy -= 16;
                        }
                        else if(tmpy == -1) {
                            tmpChunk = tmpChunk->yNeg;
                            tmpy += 16;
                        }
                        if(tmpx == 16 && tmpChunk) {
                            tmpChunk = tmpChunk->xPos;
                            tmpx -= 16;
                        }
                        else if(tmpx == -1 && tmpChunk) {
                            tmpChunk = tmpChunk->xNeg;
                            tmpx += 16;
                        }
                        if(tmpz == 16 && tmpChunk) {
                            tmpChunk = tmpChunk->zPos;
                            tmpz -= 16;
                        }
                        else if(tmpz == -1 && tmpChunk) {
                            tmpChunk = tmpChunk->zNeg;
                            tmpz += 16;
                        }
                        if(tmpChunk && tmpChunk->BlockType[tmpy][tmpx][tmpz] & 0x80) {
                            tmpShadow = 1;
                        }
                            
                    }
                    tmpShadow--;
                    /*if(this->x+x == -13 && this->y+y == 120 && this->z+z == 2 && dir == 5) {
                        int m = 0;
                        m++;
                    }*/
                    vertices[i] = 1.0f-0.5f*tmpShadow;
                    //tmp[VERTEX_SIZE*i+8] -= 0.5f*tmpShadow;
                }
                /*if(vertices[0]+vertices[2]+vertices[1]+vertices[3] == 3.5f) {
                    int i;
                    for(i = 0; i < 4; i++) {
                        if(vertices[i] == 0.5f) {
                            break;
                        }
                    }
                    vertices[(i+1)%4] = 1.5f;
                    vertices[(i+2)%4] = 2.5f;
                    vertices[(i+3)%4] = 1.5f;
                }
                else if(vertices[0]+vertices[2] < vertices[1]+vertices[3]) {
                    if(vertices[0] < vertices[2]) {
                        vertices[2] = vertices[1]+vertices[3]-vertices[0];
                    }
                    else {
                        vertices[0] = vertices[1]+vertices[3]-vertices[2];
                    }
                }
                else if(vertices[0]+vertices[2] > vertices[1]+vertices[3]) {
                    if(vertices[1] < vertices[3]) {
                        vertices[3] = vertices[0]+vertices[2]-vertices[1];
                    }
                    else {
                        vertices[1] = vertices[0]+vertices[2]-vertices[3];
                    }
                }
                else if(vertices[0]+vertices[2] == 1.5f ||
                        (vertices[0]+vertices[2] == 1.0f && (vertices[0] == 0.5f || vertices[1] == 0.5f))) {
                    for(int i = 0; i < 4; i++) {
                        if(vertices[i] == 1.0f)
                            vertices[i] = 1.5f;
                    }
                }*/
                /*if(this->x+x == 0 && this->y+y == 118 && this->z+z == -93) {
                    int m = 0;
                    m++;
                }*/
                if((vertices[0] != 1.0f && vertices[0] < vertices[1] && vertices[0] < vertices[3] && vertices[2] == 1.0f) ||
                    (vertices[0] == 1.0f && vertices[2] < vertices[1] && vertices[2] < vertices[3] && vertices[2] != 1.0f)) {
                       memcpy(tmp, alternative[dir], QUAD_SIZE*sizeof(float));
                       tmp[VERTEX_SIZE*0+8] = vertices[1];
                       tmp[VERTEX_SIZE*1+8] = vertices[2];
                       tmp[VERTEX_SIZE*2+8] = vertices[3];
                       tmp[VERTEX_SIZE*3+8] = vertices[3];
                       tmp[VERTEX_SIZE*4+8] = vertices[0];
                       tmp[VERTEX_SIZE*5+8] = vertices[1];
                   }
                else {
                    for(int i = 0; i < 4; i++) {
                        tmp[VERTEX_SIZE*i+8] = vertices[i];
                    }
                    tmp[VERTEX_SIZE*4+8] = vertices[0];
                    tmp[VERTEX_SIZE*5+8] = vertices[2];
                }
            }
        }
        else if(BlockType[y][x][z] == (char)CLOUD) {
            for(int i = 0; i < 6; i++) {
                tmp[VERTEX_SIZE*i+8] = -1.0f;
            }
        }
        
        //设置贴图坐标
        set_texture(tmp, SubChunk::BlockType[y][x][z], dir);
        
        //设置递归光照亮度
        if((BlockType[y][x][z]|0xf0) != (char)0xf0 && (BlockType[y][x][z]|0xc0) == (char)0xc0) {
            int tmpy = y+offset[dir][0];
            int tmpx = x+offset[dir][1];
            int tmpz = z+offset[dir][2];
            SubChunk *tmpChunk = this;
            if(tmpy == 16) {
                tmpChunk = yPos;
            }
            else if(tmpy == -1) {
                tmpChunk = yNeg;
                tmpy += 16;
            }
            else if(tmpx == 16) {
                tmpChunk = xPos;
            }
            else if(tmpx == -1) {
                tmpChunk = xNeg;
                tmpx += 16;
            }
            else if(tmpz == 16) {
                tmpChunk = zPos;
            }
            else if(tmpz == -1) {
                tmpChunk = zNeg;
                tmpz += 16;
            }
            if(tmpChunk) {
                char brightness = tmpChunk->brightness[tmpy%16][tmpx%16][tmpz%16];
                for(int i = 0; i < QUAD_SIZE; i += VERTEX_SIZE) {
                    tmp[i+9] = tmp[i+9]*brightness;
                }
            }
        }//十字贴图
        else {
            int offsetLumin[8][3] = {0, 0, 0, 0, 0, -1, 0, -1, 0, 0, -1, -1,
                -1, 0, 0, -1, 0, -1, -1, -1, 0, -1, -1, -1};
            for(int i = 0; i < QUAD_SIZE; i += VERTEX_SIZE) {
                char brightness = 0;
                int count = 0;
                for(int j = 0; j < 8; j++) {
                    int tmpy = y+tmp[i+1]+offsetLumin[j][0];//x,y,z应该加上点的
                    int tmpx = x+tmp[i]+offsetLumin[j][1];
                    int tmpz = z+tmp[i+2]+offsetLumin[j][2];
                    SubChunk *tmpChunk = this;
                    if(tmpy == 16) {
                        tmpChunk = yPos;
                    }
                    else if(tmpy == -1) {
                        tmpChunk = yNeg;
                        tmpy += 16;
                    }
                    if(tmpx == 16) {
                        tmpChunk = xPos;
                    }
                    else if(tmpx == -1) {
                        tmpChunk = xNeg;
                        tmpx += 16;
                    }
                    if(tmpz == 16) {
                        tmpChunk = zPos;
                    }
                    else if(tmpz == -1) {
                        tmpChunk = zNeg;
                        tmpz += 16;
                    }
                    if(tmpChunk && tmpChunk->brightness[tmpy%16][tmpx%16][tmpz%16] != 0) {
                        brightness += tmpChunk->brightness[tmpy%16][tmpx%16][tmpz%16];
                        count++;
                    }
                }
                tmp[i+9] = (count != 0)? tmp[i+9]*brightness/count:0;
            }
        }
        
        //设置真实坐标
        for(int m = 0; m < QUAD_SIZE; m = m+VERTEX_SIZE)
        {
            tmp[m] += absx;
            tmp[m+1] += absy;
            tmp[m+2] += absz;
        }
        
        Quads->insert(Quads->end(), tmp, tmp+QUAD_SIZE);
    }
}


inline void VisibleChunks::addTransQuads(int dir, int y, int x, int z) {
    //transQuads.insert(transQuads.end(), new TransQuad(x, y, z, dir));
    /*float tmp[QUAD_SIZE];
    memcpy(tmp, vertex[dir], QUAD_SIZE*sizeof(float));
    for(int m = 0; m < QUAD_SIZE; m = m+VERTEX_SIZE)
    {
        tmp[m] += x;
        tmp[m+1] += y;
        tmp[m+2] += z;
        tmp[m+6] += WATER_X;
        tmp[m+7] += WATER_Y;
    }
    transQuads.insert(transQuads.end(), tmp, tmp+QUAD_SIZE);*/
}


//广度优先搜索寻找渲染区块
void SubChunk::adjBlocksEnqueue(){
    
    if((adjVisibility&LEFT) != 0 && xNeg != NULL && xNeg->pathHistory == 0 && (pathHistory & RIGHT) == 0 && inFrustum(x-16, y, z))
    {
        xNeg->pathHistory |= LEFT|pathHistory;
        scanQueue.push(xNeg);
    }//同时满足邻接可见性+非边界+没有入队过+不是在走回头路+通过视锥体裁剪5个条件时，邻居区块入队
    if((adjVisibility&RIGHT) != 0 && xPos != NULL && xPos->pathHistory == 0 && (pathHistory & LEFT) == 0 && inFrustum(x+16, y, z))
    {
        xPos->pathHistory |= RIGHT|pathHistory;
        scanQueue.push(xPos);
    }
    if((adjVisibility&BEHIND) != 0 && zNeg != NULL && zNeg->pathHistory == 0 && (pathHistory & FRONT) == 0 && inFrustum(x, y, z-16))
    {
        zNeg->pathHistory |= BEHIND|pathHistory;
        scanQueue.push(zNeg);
    }
    if((adjVisibility&FRONT) != 0 && zPos != NULL && zPos->pathHistory == 0 && (pathHistory & BEHIND) == 0 && inFrustum(x, y, z+16))
    {
        zPos->pathHistory |= FRONT|pathHistory;
        scanQueue.push(zPos);
    }
    if((adjVisibility&DOWN) != 0 && yNeg != NULL && yNeg->pathHistory == 0 && (pathHistory & UP) == 0 &&inFrustum(x, y-16, z))
    {
        yNeg->pathHistory |= DOWN|pathHistory;
        scanQueue.push(yNeg);
    }
    if((adjVisibility&UP) != 0 && yPos != NULL && yPos->pathHistory == 0 && (pathHistory & DOWN) == 0 &&
                                                                                                       inFrustum(x, y+16, z))
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
    set<SubChunk *> redrawChunk;
    if(type == (char)AIR || type == BASE_ROCK || type == (char)WATER)
        return (char)AIR;
    char xNegType = (x == 0)? ((xNeg)? xNeg->BlockType[y][15][z]: BOUND) : BlockType[y][x-1][z];
    char xPosType = (x == 15)? ((xPos)? xPos->BlockType[y][0][z]: BOUND) : BlockType[y][x+1][z];
    char zNegType = (z == 0)? ((zNeg)? zNeg->BlockType[y][x][15]: BOUND) : BlockType[y][x][z-1];
    char zPosType = (z == 15)? ((zPos)? zPos->BlockType[y][x][0]: BOUND) : BlockType[y][x][z+1];
    char yPosType = (y == 15)? ((yPos)? yPos->BlockType[0][x][z]: BOUND) : BlockType[y+1][x][z];
    if(xNegType == (char)WATER || xPosType == (char)WATER ||
       zNegType == (char)WATER || zPosType == (char)WATER || yPosType == (char)WATER) {
        BlockType[y][x][z] = AIR;
        waterFilling(y, x, z);
    }
    else {
        BlockType[y][x][z] = AIR;
    }
    //重新计算递归光照
    if(type == (char)TORCH || type == (char)GLOWSTONE) {
        LuminousObj* tmpl = removeLuminousObj(y, x, z);
        set<SubChunk *>::iterator iter = tmpl->inflenced.begin();
        while(iter != tmpl->inflenced.end()) {
            redrawChunk.insert((*iter));
            iter++;
        }
        //delete tmpl;
    }
    else {
        updateBrightness();
        set<LuminousObj *>::iterator iter = luminousObjs.begin();
        while(iter != luminousObjs.end()) {
            set<SubChunk *>::iterator iter1 = (*iter)->inflenced.begin();
            while(iter1 != (*iter)->inflenced.end()) {
                redrawChunk.insert(*(iter1));
                iter1++;
            }
            iter++;
        }
    }
    //初始化阴影
    memset(vertexShadow, 0, 16*16*16*sizeof(unsigned short));
    
    updateQuads();
    parent->updateWater();
    if((type&0x80) == 0)
    {
        count--;
        if(x == 0 && xNeg != NULL)
        {
            //xNeg->addVertices(XPOS, y, 15, z);
            adjVisibility |= LEFT;
            xNeg->updateQuads();
            xNeg->parent->updateWater();
            redrawChunk.erase(xNeg);
            //xNeg->bufferObject.updateBuffer(false, &(xNeg->Quads[0]), xNeg->Quads.size()); //更新VBO绑定数据
        }
        if(x == 15 && xPos != NULL)
        {
            //xPos->addVertices(XNEG, y, 0, z);
            adjVisibility |= RIGHT;
            xPos->updateQuads();
            xPos->parent->updateWater();
            redrawChunk.erase(xPos);
            //xPos->bufferObject.updateBuffer(false, &(xPos->Quads[0]), xPos->Quads.size()); //更新VBO绑定数据
        }
        if(z == 0 && zNeg != NULL)
        {
            //zNeg->addVertices(ZPOS, y, x, 15);
            adjVisibility |= BEHIND;
            zNeg->updateQuads();
            zNeg->parent->updateWater();
            redrawChunk.erase(zNeg);
            //zNeg->bufferObject.updateBuffer(false, &(zNeg->Quads[0]), zNeg->Quads.size()); //更新VBO绑定数据
        }
        if(z == 15 && zPos != NULL)
        {
            //zPos->addVertices(ZNEG, y, x, 0);
            adjVisibility |= FRONT;
            zPos->updateQuads();
            zPos->parent->updateWater();
            redrawChunk.erase(zPos);
            //zPos->bufferObject.updateBuffer(false, &(zPos->Quads[0]), zPos->Quads.size()); //更新VBO绑定数据
        }
        if(y == 0 && yNeg != NULL)
        {
            //yNeg->addVertices(YPOS, 15, x, z);
            adjVisibility |= DOWN;
            yNeg->updateQuads();
            yNeg->parent->updateWater();
            redrawChunk.erase(yNeg);
            //yNeg->bufferObject.updateBuffer(true, &(yNeg->Quads[0]), yNeg->Quads.size()); //更新VBO绑定数据
        }
        if(y == 15 && yPos != NULL)
        {
            //yPos->addVertices(YNEG, 0, x, z);
            adjVisibility |= UP;
            yPos->updateQuads();
            yPos->parent->updateWater();
            redrawChunk.erase(yPos);
            //yPos->bufferObject.updateBuffer(false, &(yPos->Quads[0]), yPos->Quads.size()); //更新VBO绑定数据
        }
    }//更新子区块的可见面。如果方块非透明且处于边界，在相邻子区块中添加一个可见面，且设置该面的邻接可见性为true
    set<SubChunk *>::iterator iter = redrawChunk.begin();
    while(iter != redrawChunk.end()) {
        (*iter)->updateQuads();
        (*iter)->parent->updateWater();
        iter++;
    }
    return type;
}

//放置方块(放置或移除方块后，需要更新被改变的子区块的VAO数据)
bool SubChunk::placeBlock(char type, int dir, int y, int x, int z){
    set<SubChunk *> redrawChunk;
    if(type == (char)AIR || !(BlockType[y][x][z] == (char)AIR || BlockType[y][x][z] == (char)WATER) ||
       ((dir != YPOS || BlockType[y][x][z] == (char)WATER) && (type&0xf0) != 0xf0 && (type&0xc0) == 0xc0))
    {
        return false;
    } //如果人物手中什么都没有，不放置方块
    BlockType[y][x][z] = type;
    if(isEmpty == true)
        isEmpty = false;
    //重新计算光照
    if(type == (char)TORCH || type == (char)GLOWSTONE) {
        brightness[y][x][z] = POINT_BRIGHTNESS;
        calcBrightness(y, x, z);
        LuminousObj* tmpl = findLuminousObjs(y, x, z, this);
        set<SubChunk *>::iterator iter = tmpl->inflenced.begin();
        while(iter != tmpl->inflenced.end()) {
            redrawChunk.insert((*iter));
            iter++;
        }
    } else if((type&0x80) == 0 || type == (char)LEAF){
        updateBrightness();
        set<LuminousObj *>::iterator iter = luminousObjs.begin();
        while(iter != luminousObjs.end()) {
            set<SubChunk *>::iterator iter1 = (*iter)->inflenced.begin();
            while(iter1 != (*iter)->inflenced.end()) {
                redrawChunk.insert(*(iter1));
                iter1++;
            }
            iter++;
        }
    }
    //初始化阴影
    memset(vertexShadow, 0, 16*16*16*sizeof(unsigned short));
    updateQuads();
    parent->updateWater();
    if((type&0x80) == 0)
    {
        count++;
        if(x == 0 || x == 15 || z == 0 || z == 15 || y == 0 || y == 15) {
            updateVisibility();
        }
        if(x == 0 && xNeg != NULL) {
            xNeg->updateQuads();
            xNeg->parent->updateWater();
            redrawChunk.erase(xNeg);
        }
        if(x == 15 && xPos != NULL) {
            xPos->updateQuads();
            xPos->parent->updateWater();
            redrawChunk.erase(xPos);
        }
        if(z == 0 && zNeg != NULL) {
            zNeg->updateQuads();
            zNeg->parent->updateWater();
            redrawChunk.erase(zNeg);
        }
        if(z == 15 && zPos != NULL) {
            zPos->updateQuads();
            zPos->parent->updateWater();
            redrawChunk.erase(zPos);
        }
        if(y == 0 && yNeg != NULL) {
            yNeg->updateQuads();
            yNeg->parent->updateWater();
            redrawChunk.erase(yNeg);
        }
        if(y == 15 && yPos != NULL) {
            yPos->updateQuads();
            yPos->parent->updateWater();
            redrawChunk.erase(yPos);
        }
    } //更新子区块的可见面.如果放置的是非透明方块，更新子区块临接可见性。如果方块处于边界，那么相邻的子区块也要更新可见面
    set<SubChunk *>::iterator iter = redrawChunk.begin();
    while(iter != redrawChunk.end()) {
        (*iter)->updateQuads();
        (*iter)->parent->updateWater();
        iter++;
    }
    return true;
}

void SubChunk::calcBrightness(int y, int x, int z) {
    class Block {
        public:
        Block(int y, int x, int z, char brightness, SubChunk *s) {
            this->y = y;
            this->x = x;
            this->z = z;
            this->brightness = brightness;
            this->s = s;
        }
        int y;
        int x;
        int z;
        char brightness;
        SubChunk *s;
    };
    
    LuminousObj *luminousObj = findLuminousObjs(y, x, z, this);
    if(luminousObj == NULL) {
        luminousObj = new LuminousObj(y, x, z, this->BlockType[y][x][z], this);
    } //不存在时才插入新的光照物体
    
    bool visit[16][16][16] = {false};
    
    queue<Block> blocks;
    blocks.push(Block(y, x, z, brightness[y][x][z], this));
    int tmpx, tmpy, tmpz, difference, tmpindex;
    SubChunk *tmps, *subChunk;
    char tmpb, tmpType;
    while(!blocks.empty()) {
        Block tmp = blocks.front();
        blocks.pop();
        tmpx = tmp.x;
        tmpy = tmp.y;
        tmpz = tmp.z;
        tmps = tmp.s;
        tmps->luminousObjs.insert(luminousObj);
        luminousObj->inflenced.insert(tmps);
        //记录点光源影响的subblock和影响当前subblock的点光源
        
        
        tmpType = tmps->BlockType[tmpy][tmpx][tmpz];
        if(tmpType == (char)WATER || tmpType == (char)LEAF) {
            difference = 3;
        }
        else {
            difference = 1;
        }
        tmpb = tmp.brightness - difference;
        if(tmpb <= 0) {
            continue;
        } //光线不会继续传递，停止周围方块入队
        
        //x-
        subChunk = tmps;
        if(tmpx == 0) {
            subChunk = tmps->xNeg;
        }
        tmpindex = (tmpx-1)%16;
        if(tmpindex < 0){
            tmpindex += 16;
        }
        if(subChunk && (subChunk->BlockType[tmpy][tmpindex][tmpz]&0x80) &&
           (tmpb > subChunk->brightness[tmpy][tmpindex][tmpz] ||
           (!visit[tmpy][tmpindex][tmpz] && tmpb == subChunk->brightness[tmpy][tmpindex][tmpz]))) {
               blocks.push(Block(tmpy, tmpindex, tmpz, tmpb, subChunk));
               subChunk->brightness[tmpy][tmpindex][tmpz] = tmpb;
               visit[tmpy][tmpindex][tmpz] = true;
               //在入队以后就改变，防止方块亮度不足的情况重复入队
        }
        
        
        //x+
        subChunk = tmps;
        if(tmpx == 15){
            subChunk = tmps->xPos;
        }
        if(subChunk && (subChunk->BlockType[tmpy][(tmpx+1)%16][tmpz]&0x80) &&
           (tmpb > subChunk->brightness[tmpy][(tmpx+1)%16][tmpz] ||
            (!visit[tmpy][(tmpx+1)%16][tmpz] && tmpb == subChunk->brightness[tmpy][(tmpx+1)%16][tmpz]))) {
               blocks.push(Block(tmpy, (tmpx+1)%16, tmpz, tmpb, subChunk));
               subChunk->brightness[tmpy][(tmpx+1)%16][tmpz] = tmpb;
               visit[tmpy][(tmpx+1)%16][tmpz] = true;
               //在入队以后就改变，防止方块亮度不足的情况重复入队
           }
        
        //z-
        subChunk = tmps;
        if(tmpz == 0) {
            subChunk = tmps->zNeg;
        }
        tmpindex = (tmpz-1)%16;
        if(tmpindex < 0){
            tmpindex += 16;
        }
        if(subChunk && (subChunk->BlockType[tmpy][tmpx][tmpindex]&0x80) &&
           (tmpb > subChunk->brightness[tmpy][tmpx][tmpindex] ||
            (!visit[tmpy][tmpx][tmpindex] && tmpb == subChunk->brightness[tmpy][tmpx][tmpindex]))) {
               blocks.push(Block(tmpy, tmpx, tmpindex, tmpb, subChunk));
               subChunk->brightness[tmpy][tmpx][tmpindex] = tmpb;
               visit[tmpy][tmpx][tmpindex] = true;
               //在入队以后就改变，防止方块亮度不足的情况重复入队
           }
        
        //z+
        subChunk = tmps;
        if(tmpz == 15) {
            subChunk = tmps->zPos;
        }
        if(subChunk && (subChunk->BlockType[tmpy][tmpx][(tmpz+1)%16]&0x80) &&
           (tmpb > subChunk->brightness[tmpy][tmpx][(tmpz+1)%16] ||
            (!visit[tmpy][tmpx][(tmpz+1)%16] && tmpb == subChunk->brightness[tmpy][tmpx][(tmpz+1)%16]))) {
               blocks.push(Block(tmpy, tmpx, (tmpz+1)%16, tmpb, subChunk));
               subChunk->brightness[tmpy][tmpx][(tmpz+1)%16] = tmpb;
               visit[tmpy][tmpx][(tmpz+1)%16] = true;
               //在入队以后就改变，防止方块亮度不足的情况重复入队
           }
        
        //y-
        subChunk = tmps;
        if(tmpy == 0) {
            subChunk = tmps->yNeg;
        }
        tmpindex = (tmpy-1)%16;
        if(tmpindex < 0){
            tmpindex += 16;
        }
        if(subChunk && (subChunk->BlockType[tmpindex][tmpx][tmpz]&0x80) &&
           (tmpb > subChunk->brightness[tmpindex][tmpx][tmpz] ||
            (!visit[tmpindex][tmpx][tmpz] && tmpb == subChunk->brightness[tmpindex][tmpx][tmpz]))) {
               blocks.push(Block(tmpindex, tmpx, tmpz, tmpb, subChunk));
               subChunk->brightness[tmpindex][tmpx][tmpz] = tmpb;
               visit[tmpindex][tmpx][tmpz] = true;
               //在入队以后就改变，防止方块亮度不足的情况重复入队
           }
        
        //y+
        subChunk = tmps;
        if(tmpy == 15) {
            subChunk = tmps->yPos;
        }
        if(subChunk && (subChunk->BlockType[(tmpy+1)%16][tmpx][tmpz]&0x80) &&
           (tmpb > subChunk->brightness[(tmpy+1)%16][tmpx][tmpz] ||
            (!visit[(tmpy+1)%16][tmpx][tmpz] && tmpb == subChunk->brightness[(tmpy+1)%16][tmpx][tmpz]))) {
               blocks.push(Block((tmpy+1)%16, tmpx, tmpz, tmpb, subChunk));
               subChunk->brightness[(tmpy+1)%16][tmpx][tmpz] = tmpb;
               visit[(tmpy+1)%16][tmpx][tmpz] = true;
               //在入队以后就改变，防止方块亮度不足的情况重复入队
           }
    }
}

void SubChunk::updateBrightness() {
    set<LuminousObj *>::iterator iter=luminousObjs.begin();
    memset(brightness, 0, 16*16*16*sizeof(char)); //清空原有光照情况
    while(iter != luminousObjs.end()) {
        SubChunk *tmps = (*iter)->subChunk;
        int y = (*iter)->y;
        int x = (*iter)->x;
        int z = (*iter)->z;
        brightness[y][x][z] = POINT_BRIGHTNESS;
        tmps->calcBrightness(y, x, z);
        iter++;
    }
}

LuminousObj* SubChunk::removeLuminousObj(int y, int x, int z) {
    LuminousObj *tmp = findLuminousObjs(y, x, z, this);
    set<LuminousObj *>reCalcObj;
    if(tmp) {
        set<SubChunk *>::iterator iter=tmp->inflenced.begin();
        while(iter != tmp->inflenced.end()) {
            if((*iter)->eraseLuminousObjs(y, x, z, this)) {
                memset((*iter)->brightness, 0, 16*16*16*sizeof(char)); //清空原有光照情况
                set<LuminousObj *>::iterator iter1 = (*iter)->luminousObjs.begin();
                while (iter1 != (*iter)->luminousObjs.end()) {
                    reCalcObj.insert((*iter1));
                    iter1++;
                }
                //(*iter)->updateBrightness();
            }
            iter++;
        }
        set<LuminousObj *>::iterator reCalcIter = reCalcObj.begin();
        while(reCalcIter != reCalcObj.end()) {
            SubChunk *tmps = (*reCalcIter)->subChunk;
            int y = (*reCalcIter)->y;
            int x = (*reCalcIter)->x;
            int z = (*reCalcIter)->z;
            tmps->brightness[y][x][z] = POINT_BRIGHTNESS;
            tmps->calcBrightness(y, x, z);
            reCalcIter++;
        }
    }
    return tmp;
}

LuminousObj* SubChunk::eraseLuminousObjs(int y, int x, int z, SubChunk *s) {
    LuminousObj* obj = findLuminousObjs(y, x, z, s);
    if(obj) {
        luminousObjs.erase(obj);
    }
    return obj;
}

LuminousObj* SubChunk::findLuminousObjs(int y, int x, int z, SubChunk *s) {
    set<LuminousObj *>::iterator iter=luminousObjs.begin();
    while(iter != luminousObjs.end()) {
        int tmpy = (*iter)->y;
        int tmpx = (*iter)->x;
        int tmpz = (*iter)->z;
        SubChunk *tmps = (*iter)->subChunk;
        if(tmpy == y && tmpx == x && tmpz == z && tmps == s) {
            return (*iter);
        }
        iter++;
    }
    return NULL;
}

int prime1[OCTAVES*2] = {15731, 15791, 15761, 15749, 15727, 15731, 15791, 15761, 15749, 15797};
int prime2[OCTAVES*2] = {789221, 789101, 789137, 789169, 789227,
                        789221, 789101, 789137, 789169, 789251};
int prime3[OCTAVES*2] = {1376312589, 1376312017, 1376312087, 1376312783, 1376312857,
                        1376312589, 1376312017, 1376312087, 1376312783, 1376312689};


#define GEN_HEIGHT 0
#define GEN_CLOUD 1
int gen_choice = 0;
float Noise(int x, int y){
    static int i = 0;
    int n = x+y*57;
    i = (i+1)%(OCTAVES);
    n = (n<<13)^n;
    return (1.0-((n*(n*n*prime1[gen_choice]+prime2[gen_choice])+prime3[gen_choice])&0x7fffffff)/1073741824.0);
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
    float ft = x*3.1415926;
    float f = (1-cos(ft))*0.5;
    return a*(1-f)+b*f;
}


float InterpolatedNoise(float x, float y)
{
    int integerX = int(x);
    if(x < 0){
        integerX--;
    }
    float fractionalX = x-integerX;
    int integerY = int(y);
    if(y < 0){
        integerY--;
    }
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
float PerlinNoise(float x, float y)
{
    float total = 0;
    x = x*0.1;
    y = y*0.1;
    for(int i = 0; i < OCTAVES; i++)
    {
        float frequency = pow(2, i);
        float amplitude = pow(PERSISTENCE, i);
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
    else {
        for(int i = 0; i < 16; i++){
            subChunks[i]->updateCount();
        }
    }
}

Chunk::~Chunk(){
    for(int i = 0; i < 16; i++)
    {
        //delete subChunks[i];
    }
}

Chunk* Chunk::recycle(int x, int z){
    this->x = x;
    this->z = z;
    xNeg = xPos = zNeg = zPos = NULL;
    for(int i = 0; i < 16; i++)
    {
        subChunks[i]->recycle(i*16, x, z);
    }
    if(!readFile(to_string(x)+"_"+to_string(z))) {
        for(int i = 0; i < 16; i++) {
            memset(subChunks[i]->vertexShadow, 0, 16*16*16*sizeof(unsigned short));
            memset(subChunks[i]->brightness, 0, 16*16*16*sizeof(char));
        }
        generateMap();  //data not exist, then regenerate
    }
    else {
        for(int i = 0; i < 16; i++) {
            subChunks[i]->updateCount();
        }
    }
    return this;
}

//利用二维柏林噪声生成地图
bool Chunk::generateMap()
{
    int max = 0;
    gen_choice = GEN_HEIGHT;
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            height[i][j] = (int)(120+PerlinNoise(x+i, z+j)*32);
            if(max < height[i][j])
                max = height[i][j];
        }
    } //the height of soil layer
    maxHeight = max;
    
    
    /*int heightRock[16][16];
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            int tmpHeight = (int)(115+PerlinNoise(x+i, z+j)*32);
            if(tmpHeight >= height[i][j]-SOIL_THICKNESS)
                heightRock[i][j] = height[i][j]-SOIL_THICKNESS;
            else
                heightRock[i][j] = tmpHeight;
        }
    }*/
    
    for(int j = 0; j < 16; j++) {
     for(int k = 0; k < 16; k++) {
     subChunks[0]->BlockType[0][j][k] = BASE_ROCK;
     }
     } //generte base rock
    
    for(int m = 0; m < 16; m++)
    {
        if(m*16 > maxHeight){
            if(m*16 >= SEA_LEVEL) {
                subChunks[m]->isEmpty = true;
                memset(subChunks[m]->BlockType, AIR, 16*16*16*sizeof(char));
            }
            else{
                subChunks[m]->isEmpty = false;
                for(int i = 0; i <16; i++){
                    if(m*16+i < SEA_LEVEL)
                        memset(subChunks[m]->BlockType+16*16*i, WATER, 16*16*sizeof(char));
                    else
                        memset(subChunks[m]->BlockType+16*16*i, AIR, 16*16*sizeof(char));
                }
            }
            continue;
        }
        subChunks[m]->isEmpty = false;
        int i = (m != 0)?0:1;
        for( ; i < 16; i++)
        {
            for(int j = 0; j < 16; j++)
            {
                for(int k = 0; k < 16; k++)
                {
                    int tmpHeight = i+m*16;
                    if(tmpHeight <= height[j][k]-SOIL_THICKNESS) {
                        subChunks[m]->BlockType[i][j][k] = ROCK;
                        subChunks[m]->count++;
                    }
                    else if(tmpHeight < height[j][k]) {
                        subChunks[m]->BlockType[i][j][k] = SOIL;
                        subChunks[m]->count++; //只统计非透明砖块
                    }
                    else if(tmpHeight == height[j][k]) {
                        if (tmpHeight >= SEA_LEVEL-1) {
                            subChunks[m]->BlockType[i][j][k] = GRASSLAND;
                        } else {
                            subChunks[m]->BlockType[i][j][k] = SOIL;
                        }
                        subChunks[m]->count++;
                    }
                    else if(tmpHeight < SEA_LEVEL) {
                        subChunks[m]->BlockType[i][j][k] = WATER;
                    }
                    else {
                        subChunks[m]->BlockType[i][j][k] = AIR;
                    }
                }
            }
        }
    }
    generateHerb();
    generateCloud();
    //writeFile(to_string(xPos)+"_"+to_string(zPos));
    return true;
}

void Chunk::generateCloud() {
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            gen_choice = GEN_CLOUD;
            int tmp = (int)(PerlinNoise(x+i, z+j)*10-4);
            if(tmp >= 0) {
                int thickness1 = (tmp > 0)? 1:0;
                for(int k = 8-thickness1; k <= 8; k++) {
                    subChunks[10]->BlockType[k][i][j] = CLOUD;
                }
                subChunks[10]->isEmpty = false;
            }
        }
    } //the height of soil layer
}

void Chunk::generateHerb() {
    bool hasTree = false;
    for(int i = 0; i < 16; i++) {
        int x = i+this->x;
        int randx = glm::abs((x*(x*x*prime1[0]+prime2[0])+prime3[0])&0x7fffffff);
        for(int j = 0; j < 16; j++) {
            int z = j+this->z;
            int y = height[i][j];
            int randz = glm::abs((z*(z*z*prime1[0]+prime2[0])+prime3[0])&0x7fffffff);
            int randy = glm::abs((y*(y*y*prime1[0]+prime2[0])+prime3[0])&0x7fffffff);
            if(y > SEA_LEVEL) {
                if(glm::abs(randx*randy*randz)%64 == 1) {
                    subChunks[(y+1)/16]->BlockType[(y+1)%16][i][j] = (char)GRASS;
                }
                else if(glm::abs(randx*randy*randz)%256 == 3) {
                    subChunks[(y+1)/16]->BlockType[(y+1)%16][i][j] = (char)FLOWER1+glm::abs(x*y*z)%4;
                }
                if(hasTree == false && glm::abs(randx*randy*randz)%512 > 500 &&
                   i > 5 && i < 10 && j > 5 && j < 10) {
                    hasTree = generateTree(y, i, j);
                }
            }
        }
    }
}

int tree[4][7][7] = {0, 0, 0, 1, 0, 0, 0,
                      0, 1, 1, 1, 1, 1, 0,
                      0, 1, 1, 1, 1, 1, 0,
                      1, 1, 1, 1, 1, 1, 1,
                      0, 1, 1, 1, 1, 1, 0,
                      0, 1, 1, 1, 1, 1, 0,
                      0, 0, 0, 1, 0, 0, 0,
    
                      0, 0, 1, 1, 1, 0, 0,
                      0, 1, 1, 1, 1, 1, 0,
                      1, 1, 1, 1, 1, 1, 1,
                      1, 1, 1, 1, 1, 1, 1,
                      1, 1, 1, 1, 1, 1, 1,
                      0, 1, 1, 1, 1, 1, 0,
                      0, 0, 1, 1, 1, 0, 0,
    
                      0, 0, 0, 1, 0, 0, 0,
                      0, 1, 1, 1, 1, 1, 0,
                      0, 1, 1, 1, 1, 1, 0,
                      1, 1, 1, 1, 1, 1, 1,
                      0, 1, 1, 1, 1, 1, 0,
                      0, 1, 1, 1, 1, 1, 0,
                      0, 0, 0, 1, 0, 0, 0,
    
                      0, 0, 0, 0, 0, 0, 0,
                      0, 0, 1, 1, 1, 0, 0,
                      0, 1, 1, 1, 1, 1, 0,
                      0, 1, 1, 1, 1, 1, 0,
                      0, 1, 1, 1, 1, 1, 0,
                      0, 0, 1, 1, 1, 0, 0,
                      0, 0, 0, 0, 0, 0, 0};

bool Chunk::generateTree(int y, int x, int z) {
    if((this->x == 0 && this->z == 0) || subChunks[(y+7)/16]->isEmpty) {
        return false;
    }
    //先检查能否生成树
    int tmpy = y+4;
    SubChunk *tmp = subChunks[tmpy/16];
    for(int i = 0; i < 7; i++) {
        if(tmp->BlockType[tmpy%16][x-3+i][z-3] != (char)AIR ||
           tmp->BlockType[tmpy%16][x-3+i][z+3] != (char)AIR ||
           tmp->BlockType[tmpy%16][x-3][z+i-3] != (char)AIR ||
           tmp->BlockType[tmpy%16][x+3][z+i-3] != (char)AIR) {
            return false;
        }
    }
    //生成叶子
    for(int i = 0; i < 4; i++) {
        int tmpy = y+4+i;
        for(int j = 0; j < 7; j++) {
            int tmpx = x+j-3;
            for(int k = 0; k < 7; k++) {
                int tmpz = z+k-3;
                if(tree[i][j][k] == 1) {
                    subChunks[tmpy/16]->BlockType[tmpy%16][tmpx][tmpz] = LEAF;
                }
            }
        }
    }
    //生成树干
    subChunks[y/16]->BlockType[y%16][x][z] = SOIL;
    for(int k = 1; k < 6; k++) {
        subChunks[(y+k)/16]->BlockType[(y+k)%16][x][z] = TRUNK;
    }
    return true;
}

bool Chunk::readFile(string filePath){
    ifstream fin(filePath,ios::binary);
    if(!fin)
        return false;
    else
    {
        for(int i = 0; i < 16; i++) {
            fin.read((char*)subChunks[i]->BlockType, 16*16*16*sizeof(char));
            fin.read((char*)subChunks[i]->vertexShadow, 16*16*16*sizeof(unsigned short));
            fin.read((char*)subChunks[i]->brightness, 16*16*16*sizeof(char));
        }
        fin.close();
        return true;
    }
}

bool Chunk::writeFile(string filePath){
    ofstream fout(filePath,ios::binary);
    if(!fout)
        return false;
    else
    {
        for(int i = 0; i < 16; i++) {
            fout.write((char*)subChunks[i]->BlockType, 16*16*16*sizeof(char));
            fout.write((char*)subChunks[i]->vertexShadow, 16*16*16*sizeof(unsigned short));
            fout.write((char*)subChunks[i]->brightness, 16*16*16*sizeof(char));
        }
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
        subChunks[i]->updateNeighbor(this, dir);
    } //更新所有子区块的邻居
}


void Chunk::addVertices(int dir, int y, int x, int z) {
    float tmp[QUAD_SIZE];
    memcpy(tmp, waterVertices, QUAD_SIZE*sizeof(float));
    int offsetLumin[8][3] = {0, 0, 0, 0, 0, -1, 0, -1, 0, 0, -1, -1,
        -1, 0, 0, -1, 0, -1, -1, -1, 0, -1, -1, -1};
    for(int i = 0; i < QUAD_SIZE; i += VERTEX_SIZE) {
        char brightness = 0;
        int count = 0;
        for(int j = 0; j < 8; j++) {
            int tmpy = y+tmp[i+1]+offsetLumin[j][0];
            int tmpx = x-this->x+tmp[i]+offsetLumin[j][1];
            int tmpz = z-this->z+tmp[i+2]+offsetLumin[j][2];
            Chunk *tmpChunk = this;
            if(tmpx == 16) {
                tmpChunk = xPos;
            }
            else if(tmpx == -1) {
                tmpChunk = xNeg;
                tmpx += 16;
            }
            if(tmpz == 16) {
                tmpChunk = zPos;
            }
            else if(tmpz == -1) {
                tmpChunk = zNeg;
                tmpz += 16;
            }
            if(tmpChunk && tmpChunk->subChunks[y/16]->brightness[tmpy%16][tmpx%16][tmpz%16] != 0) {
                brightness += tmpChunk->subChunks[y/16]->brightness[tmpy%16][tmpx%16][tmpz%16];
                count++;
            }
        }
        tmp[i+9] = (count != 0)? tmp[i+9]*brightness/count:0;
    }
    for(int m = 0; m < QUAD_SIZE; m = m+VERTEX_SIZE)
    {
        tmp[m] += x;
        tmp[m+1] += y;
        tmp[m+2] += z;
    }
    for(int m = 0; m < QUAD_SIZE; m = m+VERTEX_SIZE) {
        tmp[m+6] += WATER_X;
        tmp[m+7] += WATER_Y;
    }
    Water.insert(Water.end(), tmp, tmp+QUAD_SIZE);
}

bool SubChunk::waterFilling(int y, int x, int z) {
    if(BlockType[y][x][z] != (char)AIR || y >= SEA_LEVEL)
        return false;
    BlockType[y][x][z] = WATER;
    class Coordinate{
        public:
        int x;
        int y;
        int z;
        //int visited;
        //Coordinate() {visited = 0;}
        Coordinate* set(int y, int x, int z) {
            this->x = x;
            this->y = y;
            this->z = z;
            //visited = 1;
            return this;
        }
    };
    queue<Coordinate *> scanQueue;
    Coordinate *tmp;
    Coordinate blocks[16][16][16];
    scanQueue.push(blocks[y][x][z].set(y, x, z));
    while(!scanQueue.empty()) {
        tmp = scanQueue.front();
        scanQueue.pop();
        //BlockType[tmp->y][tmp->x][tmp->z] = WATER;
        if(tmp->x == 0) {
            if(xNeg != NULL)
            xNeg->waterFilling(tmp->y, 15, tmp->z);
        }
        else if(BlockType[tmp->y][tmp->x-1][tmp->z] == (char)AIR) {
            scanQueue.push(blocks[tmp->y][tmp->x-1][tmp->z].set(tmp->y, tmp->x-1, tmp->z));
            BlockType[tmp->y][tmp->x-1][tmp->z] = WATER;
        }
        if(tmp->x == 15) {
            if(xPos != NULL)
            xPos->waterFilling(tmp->y, 0, tmp->z);
        }
        else if(BlockType[tmp->y][tmp->x+1][tmp->z] == (char)AIR) {
            scanQueue.push(blocks[tmp->y][tmp->x+1][tmp->z].set(tmp->y, tmp->x+1, tmp->z));
            BlockType[tmp->y][tmp->x+1][tmp->z] = WATER;
        }
        if(tmp->z == 0) {
            if(zNeg != NULL)
            zNeg->waterFilling(tmp->y, tmp->x, 15);
        }
        else if(BlockType[tmp->y][tmp->x][tmp->z-1] == (char)AIR) {
            scanQueue.push(blocks[tmp->y][tmp->x][tmp->z-1].set(tmp->y, tmp->x, tmp->z-1));
            BlockType[tmp->y][tmp->x][tmp->z-1] = WATER;
        }
        if(tmp->z == 15) {
            if(zPos != NULL)
            zPos->waterFilling(tmp->y, tmp->x, 0);
        }
        else if(BlockType[tmp->y][tmp->x][tmp->z+1] == (char)AIR) {
            scanQueue.push(blocks[tmp->y][tmp->x][tmp->z+1].set(tmp->y, tmp->x, tmp->z+1));
            BlockType[tmp->y][tmp->x][tmp->z+1] = WATER;
        }
        if(tmp->y == 0){
            if(yNeg != NULL)
            yNeg->waterFilling(15, tmp->x, tmp->z);
        }
        else if(BlockType[tmp->y-1][tmp->x][tmp->z] == (char)AIR) {
            scanQueue.push(blocks[tmp->y-1][tmp->x][tmp->z].set(tmp->y-1, tmp->x, tmp->z));
            BlockType[tmp->y-1][tmp->x][tmp->z] = WATER;
        }
        if(tmp->y == 15){
            if(yPos != NULL)
            yPos->waterFilling(0, tmp->x, tmp->z);
        }
        else if(BlockType[tmp->y+1][tmp->x][tmp->z] == (char)AIR && tmp->y+1+this->y < SEA_LEVEL) {
            scanQueue.push(blocks[tmp->y+1][tmp->x][tmp->z].set(tmp->y+1, tmp->x, tmp->z));
            BlockType[tmp->y+1][tmp->x][tmp->z] = WATER;
        }
    }
    if(SEA_LEVEL <= this->y+y+16 && this->y+y < SEA_LEVEL)
    parent->updateWater();
    return true;
}

void Chunk::addTransBlock(char type, int y, int x, int z) {
    if((type&0xc0) == 0xc0) {
        //十字贴图
        addVertices(XCENTER, y, x, z);
        addVertices(ZCENTER, y, x, z);
    }
    else if(type&0x80){
        //立方体贴图
        for(int i = XNEG; i < YPOS; i++) {
            addVertices(i, y, x, z);
        }
    }
}

void Chunk::updateTransQuads() {
    /*map<float, TransQuad*> sorted;
    for(int i = 0; i < transQuads.size(); i++){
        float distance = glm::length(cameraPos-transQuads[i]->pos);
        sorted[distance] = transQuads[i];
    }*/
}

void Chunk::updateWater() {
    Water.clear();
    Water.shrink_to_fit();
    int index = (SEA_LEVEL-1)/16;
    int offset = (SEA_LEVEL-1)%16;
    for(int i = 0; i <16; i++) {
        for(int j = 0; j < 16; j++) {
            if(subChunks[index]->BlockType[offset][i][j] == (char)WATER) {
                addVertices(YPOS, SEA_LEVEL-1, i+x, j+z);
            }
        }
    }
    bufferObject.updateBuffer(true, &Water[0], Water.size());
}

VisibleChunks::VisibleChunks(float x, float y, float z){
    int ChunkX, ChunkZ;
    int SubChunkIndex;
    
    HoldBlock.newHoldBlock();
    initBlockInfo();

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
            Chunks[i][j]->updateWater();
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
    for(int i = 0; i < 2*RADIUS+1; i++){
        for(int j = 0; j < 2*RADIUS+1; j++){
            //Chunks[i][j]->writeFile(to_string(Chunks[i][j]->x)+"_"+to_string(Chunks[i][j]->z));
            //delete Chunks[i][j];
        }
    }
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
            //tmp->writeFile(to_string(tmp->x)+"_"+to_string(tmp->z));
            //delete tmp;
            Chunks[0][i] = tmp->recycle(ChunkX-RADIUS*16, ChunkZ+(i-RADIUS)*16);
            //Chunks[0][i] = new Chunk(ChunkX-RADIUS*16, ChunkZ+(i-RADIUS)*16);
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
            //tmp->writeFile(to_string(tmp->x)+"_"+to_string(tmp->z));
            //delete tmp;
            Chunks[2*RADIUS][i] = tmp->recycle(ChunkX+RADIUS*16, ChunkZ+(i-RADIUS)*16);
            //new Chunk(ChunkX+RADIUS*16, ChunkZ+(i-RADIUS)*16);
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
            //tmp->writeFile(to_string(tmp->x)+"_"+to_string(tmp->z));
            //delete tmp;
            Chunks[i][2*RADIUS] = tmp->recycle(ChunkX+(i-RADIUS)*16, ChunkZ+RADIUS*16);
            //Chunks[i][2*RADIUS] = new Chunk(ChunkX+(i-RADIUS)*16, ChunkZ+RADIUS*16);
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
            //tmp->writeFile(to_string(tmp->x)+"_"+to_string(tmp->z));
            //delete tmp;
            Chunks[i][0] = tmp->recycle(ChunkX+(i-RADIUS)*16, ChunkZ-RADIUS*16);
            //Chunks[i][0] = new Chunk(ChunkX+(i-RADIUS)*16, ChunkZ-RADIUS*16);
        }
        dir = BEHIND;
    } //向后走时，所有区块向前移动一格
    curChunk = Chunks[RADIUS][RADIUS];
    curSubChunk = curChunk->subChunks[SubChunkIndex]; //更新当前区块和子区块
    //transQuads.clear();
    if(dir)
        updateQuads(dir); //根据移动方向，更新边界区块的可见面
    return true;
}

void VisibleChunks::updateQuads(int dir){
    //transQuads.clear();
    updateNeighbor(dir);
    if(dir == LEFT)
    {
        for(int i = 0; i < 2*RADIUS+1; i++)
        {
            Chunks[0][i]->updateWater();
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
            Chunks[2*RADIUS][i]->updateWater();
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
            Chunks[i][0]->updateWater();
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
            Chunks[i][2*RADIUS]->updateWater();
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

queue<SubChunk*> VisibleChunks::getRenderQueue(){
    return renderQueue;
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
            if(tmp->QuadSize){
                renderQueue.push(tmp);
                if(tmp->y == 240) {
                    int m = 0;
                    m++;
                }
            }
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
        int visited;
        Coordinate() {visited = 0;}
        Coordinate* set(int y, int x, int z) {
            this->x = x;
            this->y = y;
            this->z = z;
            visited = 1;
            return this;
        }
    };
    queue<Coordinate *> s;
    Coordinate *tmp;
    Coordinate coordinates[16][16][16];
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
    
    s.push(coordinates[y][x][z].set(y,x,z));
    
    while(!s.empty())
    {
        tmp = s.front();
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
                   coordinates[tmpy][tmpx][tmpz].visited == 0)
                {
                    s.push(coordinates[tmpy][tmpx][tmpz].set(tmpy,tmpx,tmpz));
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
void VisibleChunks::draw(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection, Shader& Block_Shader, unsigned int texture_pic, unsigned int depthMap_pic, unsigned int skybox, glm::mat4 lightSpaceMatrix, glm::vec3 lightDirection, glm::vec3 chosen_block_pos, float broken_scale, glm::vec3 Sun_Moon_light, glm::vec3 Ambient_light, bool isDaylight, float dayTime, float starIntensity, bool eye_in_water) {
    calcFrustumPlane(view, projection);
    cout<<"y:"<<(int)cameraPos.y<<" x:"<<(int)cameraPos.x<<" z:"<<(int)cameraPos.z<<endl;
    updataChunks((int)cameraPos.y, (int)cameraPos.x, (int)cameraPos.z);
    Block_Shader.use();
    Block_Shader.setMat4("view", view);
    Block_Shader.setMat4("projection", projection);
    Block_Shader.setVec3("sunlight.lightDirection", lightDirection);
    Block_Shader.setVec3("sunlight.ambient", Ambient_light);
    Block_Shader.setVec3("sunlight.lightambient", Sun_Moon_light);
    Block_Shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    Block_Shader.setVec3("chosen_block_pos", chosen_block_pos);
    float broken_texture_x = floor(broken_scale*10)/10;
    Block_Shader.setFloat("broken_texture_x", broken_texture_x);
    Block_Shader.setVec3("cameraPos", cameraPos);
    Block_Shader.setFloat("DayPos", dayTime/24);
    Block_Shader.setBool("isDaylight", isDaylight);
    Block_Shader.setFloat("starIntensity", starIntensity);
    Block_Shader.setBool("eye_in_water", eye_in_water);
    Block_Shader.setFloat("noFogRadius", RADIUS*11);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_pic);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap_pic);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, skybox);
    drawNormQuads(cameraPos, Block_Shader);
    drawTransQuads(cameraPos, Block_Shader);
}

void VisibleChunks::drawDepth(Shader& Depth_Shader, unsigned int texture_pic) {

    /*if(flag == 0){
     //getRenderingSubChunks((int)cameraPos.y, (int)cameraPos.x, (int)cameraPos.z);//float为负数时候怎么rounding？？？？
     flag = 1;
     }*/
    //model = glm::translate(model, position);
    SubChunk *tmp;
    glm::mat4 model(1);
    Depth_Shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_pic);
    for(int i = 0; i < renderQueue.size(); i++)
    {
        tmp = renderQueue.front();
        glBindVertexArray(tmp->bufferObject.getVAO());
        glDrawArrays(GL_TRIANGLES, 0, (int)(tmp->QuadSize/VERTEX_SIZE));
        renderQueue.pop();
        //renderQueue.push(tmp);
    }
}

//计算视锥体各个面的方程
void VisibleChunks::calcFrustumPlane(glm::mat4 view, glm::mat4 projection){
    glm::mat4 matrix = view*projection;
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            frustumPlanes[i][j] = matrix[3][j]+glm::pow(-1, i)*matrix[i/2][j];
        }
    }//left, right, bottom, top, near, far
}

void SubChunk::set_texture(float* tmp, char type, int dir) {
    BlockInfo blockInfo = BlockInfoMap[type];
    for(int m = 0; m < QUAD_SIZE; m = m+VERTEX_SIZE)
    {
        if (dir == YPOS) {
            tmp[m+6] += blockInfo.y_pos_x;
            tmp[m+7] += blockInfo.y_pos_y;
        } else if (dir == YNEG) {
            tmp[m+6] += blockInfo.y_neg_x;
            tmp[m+7] += blockInfo.y_neg_y;
        } else if (dir == XPOS){
            tmp[m+6] += blockInfo.x_pos_x;
            tmp[m+7] += blockInfo.x_pos_y;
        } else if (dir == XNEG){
            tmp[m+6] += blockInfo.x_neg_x;
            tmp[m+7] += blockInfo.x_neg_y;
        } else if (dir == ZPOS){
            tmp[m+6] += blockInfo.z_pos_x;
            tmp[m+7] += blockInfo.z_pos_y;
        } else if (dir == ZNEG){
            tmp[m+6] += blockInfo.z_neg_x;
            tmp[m+7] += blockInfo.z_neg_y;
        } else {
            tmp[m+6] += blockInfo.y_pos_x;
            tmp[m+7] += blockInfo.y_pos_y;
        }
    }
}

void VisibleChunks::drawNormQuads(glm::vec3 cameraPos, Shader& Block_Shader){
    SubChunk *tmp;
    getRenderingSubChunks((int)cameraPos.y, (int)cameraPos.x, (int)cameraPos.z);//float为负数时候怎么rounding？？？？
    for(int i = 0; i < renderQueue.size(); i++)
    {
        tmp = renderQueue.front();
        glBindVertexArray(tmp->bufferObject.getVAO());
        glDrawArrays(GL_TRIANGLES, 0, (int)(tmp->QuadSize/VERTEX_SIZE));
        renderQueue.pop();
        renderQueue.push(tmp);
    }
}

void VisibleChunks::drawTransQuads(glm::vec3 cameraPos, Shader& Block_Shader){
    //Block *tmp = new Block();
    glm::mat4 model(1);
    Block_Shader.setMat4("model", model);
    //tmp->updateBuffer(true, &transQuads[0], transQuads.size());
    //glBindVertexArray(tmp->getVAO());
    //glDrawArrays(GL_TRIANGLES, 0, (int)(transQuads.size()/VERTEX_SIZE));
    //delete tmp;
    for(int i = 0; i <= 2*RADIUS; i++) {
        for(int j = 0; j <= 2*RADIUS; j++) {
            glBindVertexArray(Chunks[i][j]->bufferObject.getVAO());
            glDrawArrays(GL_TRIANGLES, 0, (int)(Chunks[i][j]->Water.size()/VERTEX_SIZE));
        }
    }
    /*
     map<float, TransQuad*> sorted;
     for(int i = 0; i < transQuads.size(); i++){
        float distance = glm::length(cameraPos-transQuads[i]->pos);
        sorted[distance] = transQuads[i];
    }
    for(map<float,TransQuad*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
    {
        tmp->updateBuffer(true, it->second->vertices, QUAD_SIZE);
        glBindVertexArray(tmp->getVAO());
        glDrawArrays(GL_TRIANGLES, 0, QUAD_SIZE/VERTEX_SIZE);
    }*/
    /*for(int i = 0; i < transQuads.size(); i+=QUAD_SIZE) {
        tmp->updateBuffer(true, &transQuads[i], QUAD_SIZE);
        glBindVertexArray(tmp->getVAO());
        glDrawArrays(GL_TRIANGLES, 0, QUAD_SIZE/VERTEX_SIZE);
    }*/
    //delete tmp;
}

char VisibleChunks::getBlockType(int y, int x, int z){
    int yIndex = y/16;
    
    int xOffset = x-Chunks[0][0]->x;
    int xIndex = xOffset/16;
    
    int zOffset = z-Chunks[0][0]->z;
    int zIndex = (zOffset)/16;
    
    if(yIndex < 0 || yIndex > 15 ||
       xIndex < 0 || xIndex > 2*RADIUS ||
       zIndex < 0 || zIndex > 2*RADIUS)
        return 0;
    else
        return Chunks[xIndex][zIndex]->subChunks[yIndex]->BlockType[y%16][xOffset%16][zOffset%16];
}

bool VisibleChunks::placeBlock(glm::vec3 cameraPos, glm::vec3 cameraFront, char type) {
    glm::vec3 clickPos = cameraPos;
    int prex = INT_MAX, prey = INT_MAX, prez = INT_MAX;
    if(type == (char)AIR || type == (char)WATER){
        return false;
    }
    
    for(float i = 0.0f; i < STEVE_FETCH_RADIUS; i += 0.05f){
        clickPos += 0.05f*cameraFront;
        int x = (clickPos.x >= 0)? (int)clickPos.x : (int)clickPos.x-1;
        int y = (clickPos.y >= 0)? (int)clickPos.y : (int)clickPos.y-1;
        int z = (clickPos.z >= 0)? (int)clickPos.z : (int)clickPos.z-1;
        if(x == prex && y == prey && z == prez) {
            continue;
        }
        else {
            prex = x;
            prey = y;
            prez = z;
        }
        
        
        char tmpType = getBlockType(y, x, z);
        int side = 0;
        if(tmpType == (char)AIR || tmpType == (char)WATER)
            continue; //必须有相邻块
        else if((tmpType&0xf0) != 0xf0 && (tmpType&0xc0) == 0xc0) {
            float distance = glm::abs((y-cameraPos.y)/cameraFront.y);
            float tmpx = cameraPos.x+distance*cameraFront.x-x;
            //float yOffset = cameraPos.y+distance*cameraFront.y-y;
            float tmpz = cameraPos.z+distance*cameraFront.z-z;
            if(0 > tmpx || tmpx > 1 || 0 > tmpz || tmpz > 1){
                continue;
            }
            else {
                return false;
            }
        }
        else {
            float shortest = 100.0f;
            int coefficient[6][3] = {{0, 1, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 1}, {1, 0, 0}, {1, 0, 0}};
            float addition[6][3] = {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}, {0, 0, 1}, {0, 0, 0}, {1, 0, 0}};
            for(int j = 0; j < 6; j++) {
                float distance = coefficient[j][1]*glm::abs((addition[j][1]+x-cameraPos.x)/cameraFront.x) +
                coefficient[j][2]*glm::abs((addition[j][2]+z-cameraPos.z)/cameraFront.z) +
                coefficient[j][0]*glm::abs((addition[j][0]+y-cameraPos.y)/cameraFront.y);
                float xOffset = cameraPos.x+distance*cameraFront.x-x;
                float yOffset = cameraPos.y+distance*cameraFront.y-y;
                float zOffset = cameraPos.z+distance*cameraFront.z-z;
                if(distance < shortest && 0 <= xOffset && xOffset <= 1 &&
                   0 <= yOffset && yOffset <= 1 &&
                   0 <= zOffset && zOffset <= 1){
                    shortest = distance;
                    side = j;
                }
            }
        }
        int offset[6][3] = {{0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1}, {-1, 0, 0}, {1, 0, 0}};
        x += offset[side][1];
        z += offset[side][2];
        y += offset[side][0];
        
        
        if((cameraPos.x+STEVE_RADIUS >= x && cameraPos.x-STEVE_RADIUS <= x+1) &&
           (cameraPos.y+STEVE_HEIGHT-STEVE_EYE_HEIGHT >= y && cameraPos.y-STEVE_EYE_HEIGHT <= y+1) &&
           (cameraPos.z+STEVE_RADIUS >= z && cameraPos.z-STEVE_RADIUS <= z+1))
            continue; //不能和人物重合
        
        int yIndex = y/16;
        
        int xOffset = x-Chunks[0][0]->x;
        int xIndex = xOffset/16;
        
        int zOffset = z-Chunks[0][0]->z;
        int zIndex = (zOffset)/16;
        
        
        if(Chunks[xIndex][zIndex]->subChunks[yIndex]->placeBlock(type, side, y%16, xOffset%16, zOffset%16))
            return true;
    }
    return false;
}

char VisibleChunks::removeBlock(glm::vec3 cameraPos, glm::vec3 cameraFront) {
    glm::vec3 clickPos = cameraPos;
    int prex = INT_MAX, prey = INT_MAX, prez = INT_MAX;
    for(float i = 0.0f; i < STEVE_FETCH_RADIUS; i += 0.05f){
        clickPos += 0.05f*cameraFront;
        int x = (clickPos.x >= 0)? (int)clickPos.x : (int)clickPos.x-1;
        int y = (clickPos.y >= 0)? (int)clickPos.y : (int)clickPos.y-1;
        int z = (clickPos.z >= 0)? (int)clickPos.z : (int)clickPos.z-1;
        
        if(x == prex && y == prey && z == prez) {
            continue;
        }
        else {
            prex = x;
            prey = y;
            prez = z;
        }
        
        int yIndex = y/16;
        
        int xOffset = x-Chunks[0][0]->x;
        int xIndex = xOffset/16;
        
        int zOffset = z-Chunks[0][0]->z;
        int zIndex = (zOffset)/16;
        char type = Chunks[xIndex][zIndex]->subChunks[yIndex]->BlockType[y%16][xOffset%16][zOffset%16];
        
        if((type&0xf0) != 0xf0 && (type&0xc0) == 0xc0) {
            float distance = glm::abs((y-cameraPos.y)/cameraFront.y);
            float tmpx = cameraPos.x+distance*cameraFront.x-x;
            //float yOffset = cameraPos.y+distance*cameraFront.y-y;
            float tmpz = cameraPos.z+distance*cameraFront.z-z;
            if(0 <= tmpx && tmpx <= 1 && 0 <= tmpz && tmpz <= 1){
                Chunks[xIndex][zIndex]->subChunks[yIndex]->removeBlock(y%16, xOffset%16, zOffset%16);
                return type;
            }
        }
        else if(type != (char)AIR  && type != (char)WATER) {
            Chunks[xIndex][zIndex]->subChunks[yIndex]->removeBlock(y%16, xOffset%16, zOffset%16);
            return type;
        }
    }
    return (char)AIR;
}

glm::vec3 VisibleChunks::accessibleBlock(glm::vec3 cameraPos, glm::vec3 cameraFront){
    glm::vec3 clickPos = cameraPos;
    int prex = INT_MAX, prey = INT_MAX, prez = INT_MAX;
    for(float i = 0.0f; i < STEVE_FETCH_RADIUS; i += 0.05f){
        clickPos += 0.05f*cameraFront;
        int x = (clickPos.x >= 0)? (int)clickPos.x : (int)clickPos.x-1;
        int y = (clickPos.y >= 0)? (int)clickPos.y : (int)clickPos.y-1;
        int z = (clickPos.z >= 0)? (int)clickPos.z : (int)clickPos.z-1;
        
        if(x == prex && y == prey && z == prez) {
            continue;
        }
        else {
            prex = x;
            prey = y;
            prez = z;
        }
        char type = getBlockType(y, x, z);
        
        if((type&0xf0) != 0xf0 && (type&0xc0) == 0xc0) {
            float distance = glm::abs((y-cameraPos.y)/cameraFront.y);
            float xOffset = cameraPos.x+distance*cameraFront.x-x;
            //float yOffset = cameraPos.y+distance*cameraFront.y-y;
            float zOffset = cameraPos.z+distance*cameraFront.z-z;
            if(0 <= xOffset && xOffset <= 1 && 0 <= zOffset && zOffset <= 1){
                return glm::vec3(x, y, z);
            }
        }
        else if(type != (char)AIR && type != (char)WATER)
            return glm::vec3(x, y, z);
    }
    return cameraPos;
}

void VisibleChunks::initBlockInfo() {
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)GRASS, BlockInfo("GRASS", 0.05, GRASS_X, GRASS_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)TORCH, BlockInfo("TORCH", 0.2, TORCH_X, TORCH_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)GLASS, BlockInfo("GLASS", 1.5, GLASS_X, GLASS_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)GLOWSTONE, BlockInfo("GLOWSTONE", 2, GLOWSTONE_X, GLOWSTONE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)LEAF, BlockInfo("LEAF", 0.2, LEAF_X, LEAF_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)WATER, BlockInfo("WATER", 99999, WATER_X, WATER_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)ROCK, BlockInfo("ROCK", 7.5, ROCK_X, ROCK_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)SOIL, BlockInfo("SOIL", 0.75, SOIL_X, SOIL_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)GRASSLAND, BlockInfo("GRASSLAND", 0.9, GRASSLAND_TOP_X, GRASSLAND_TOP_Y, SOIL_X, SOIL_Y, GRASSLAND_SIDE_X, GRASSLAND_SIDE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)DIAMAND_ORE, BlockInfo("DIAMAND_ORE", 99999, DIAMAND_ORE_X, DIAMAND_ORE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)BASE_ROCK, BlockInfo("BASE_ROCK", 99999, BASE_ROCK_X, BASE_ROCK_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)LEAF, BlockInfo("LEAF", 0.35, LEAF_X, LEAF_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)TRUNK, BlockInfo("TRUNK", 3, TRUNK_TOP_X, TRUNK_TOP_Y, TRUNK_TOP_X, TRUNK_TOP_Y, TRUNK_SIDE_X, TRUNK_SIDE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)WOOD, BlockInfo("WOOD", 2, WOOD_X, WOOD_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)RED_WOOD, BlockInfo("RED_WOOD", 2, RED_WOOD_X, RED_WOOD_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)TINT_WOOD, BlockInfo("TINT_WOOD", 2, TINT_WOOD_X, TINT_WOOD_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)DARK_WOOD, BlockInfo("DARK_WOOD", 2, DARK_WOOD_X, DARK_WOOD_Y)));
    
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)BRICK, BlockInfo("BRICK", 2, BRICK_X, BRICK_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)SAND, BlockInfo("SAND", 2, SAND_X, SAND_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)COAL_ORE, BlockInfo("COAL_ORE", 2, COAL_ORE_X, COAL_ORE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)GOLD_ORE, BlockInfo("GOLD_ORE", 2, GOLD_ORE_X, GOLD_ORE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)IRON_ORE, BlockInfo("IRON_ORE", 2, IRON_ORE_X, IRON_ORE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)EMERALD_ORE, BlockInfo("EMERALD_ORE", 2, EMERALD_ORE_X, EMERALD_ORE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)REDSTONE_ORE, BlockInfo("REDSTONE_ORE", 2, REDSTONE_ORE_X, REDSTONE_ORE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)TOOLBOX, BlockInfo("TOOLBOX", 2, TOOLBOX_TOP_X, TOOLBOX_TOP_Y, TOOLBOX_TOP_X, TOOLBOX_TOP_Y, TOOLBOX_SIDE1_X, TOOLBOX_SIDE1_Y, TOOLBOX_SIDE2_X, TOOLBOX_SIDE2_Y, TOOLBOX_SIDE1_X, TOOLBOX_SIDE1_Y, TOOLBOX_SIDE2_X, TOOLBOX_SIDE2_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)SMELTER, BlockInfo("SMELTER", 2, SMELTER_TOP_X, SMELTER_TOP_Y, SMELTER_TOP_X, SMELTER_TOP_Y, SMELTER_SIDE1_X, SMELTER_SIDE1_Y, SMELTER_SIDE2_X, SMELTER_SIDE2_Y, SMELTER_SIDE2_X, SMELTER_SIDE2_Y, SMELTER_SIDE2_X, SMELTER_SIDE2_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)WATERMELON, BlockInfo("WATERMELON", 2, WATERMELON_TOP_X, WATERMELON_TOP_Y, WATERMELON_TOP_X, WATERMELON_TOP_Y, WATERMELON_SIDE_X, WATERMELON_SIDE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)PUMPKIN, BlockInfo("PUMPKIN", 2, PUMPKIN_TOP_X, PUMPKIN_TOP_Y, PUMPKIN_TOP_X, PUMPKIN_TOP_Y, PUMPKIN_SIDE_X, PUMPKIN_SIDE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)WHITE_WOOL, BlockInfo("WHITE_WOOL", 2, WHITE_WOOL_X, WHITE_WOOL_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)TNT, BlockInfo("TNT", 2, TNT_TOP_X, TNT_TOP_Y, TNT_BOTTOM_X, TNT_BOTTOM_Y, TNT_SIDE_X, TNT_SIDE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)COBBLESTONE, BlockInfo("COBBLESTONE", 2, COBBLESTONE_X, COBBLESTONE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)MOSSY_COBBLESTONE, BlockInfo("MOSSY_COBBLESTONE", 2, MOSSY_COBBLESTONE_X, MOSSY_COBBLESTONE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)STONE_BRICK, BlockInfo("STONE_BRICK", 2, STONE_BRICK_X, STONE_BRICK_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)QUARTZ, BlockInfo("QUARTZ", 2, QUARTZ_TOP_X, QUARTZ_TOP_Y, QUARTZ_TOP_X, QUARTZ_TOP_Y, QUARTZ_SIDE_X, QUARTZ_SIDE_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)GOLD, BlockInfo("GOLD", 2, GOLD_X, GOLD_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)CLOUD, BlockInfo("CLOUD", 99999, CLOUD_X, CLOUD_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)FLOWER1, BlockInfo("FLOWER1", 0.05, FLOWER1_X, FLOWER1_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)FLOWER2, BlockInfo("FLOWER2", 0.05, FLOWER2_X, FLOWER2_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)FLOWER3, BlockInfo("FLOWER3", 0.05, FLOWER3_X, FLOWER3_Y)));
    BlockInfoMap.insert(std::map<char, BlockInfo> :: value_type((char)FLOWER4, BlockInfo("FLOWER4", 0.05, FLOWER4_X, FLOWER4_Y)));
}
