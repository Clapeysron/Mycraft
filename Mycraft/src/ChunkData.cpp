//
//  ChunkData.cpp
//  MineCraft
//
//  Created by apple on 17/11/14.
//  Copyright © 2017年 apple. All rights reserved.
//

#include "ChunkData.hpp"
#define SOIL_THICKNESS 5

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

bool ChunkData::generateMap(bool isSea, int seaLevel)
{
    char max = 0;
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            height[i][j] = (int)(120+PerlinNoise(xPos+i, yPos+j, 0.1, 5)*64);
            if(max < height[i][j])
                max = height[i][j];
        }
    } //the height of soil layer
    maxHeight = max;
    
    
    char heightRock[16][16];
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            char tmpHeight = (int)(115+PerlinNoise(xPos+i, yPos+j, 0.1, 5)*64);
            if(tmpHeight >= height[i][j]-SOIL_THICKNESS)
                heightRock[i][j] = height[i][j]-SOIL_THICKNESS;
            else
                heightRock[i][j] = tmpHeight;
        }
    } //the height of rock layer
    
    for(int m = 0; m <= maxHeight>>4; m++)
    {
        subChunks[m].isEmpty = false;
        for(int i = 0; i < 16; i++)
        {
            for(int j = 0; j < 16; j++)
            {
                for(int k = 0; k < 16; k++)
                {
                    subChunks[m].BlockData[i][j][k] = (i+(m<<4)<=height[j][k])?
                    ((i+(m<<4)<=heightRock[j][k])?ROCK:SOIL):AIR;
                }
            }
        }
    }
    return true;
}

char* ChunkData::readChunk()
{
    char *ret = (char *)malloc(16*16*256*sizeof(char));
    int size = 16*16*16;
    for(int i = 0; i < 16; i++)
        memcpy(ret+i*size, subChunks[i].BlockData, size*sizeof(char));
    return ret;
}

//void ChunkData::draw_chunk(glm::vec3 position, glm::mat4 view, glm::mat4 projection, Shader& Block_Shader, char type) {
//    for(int i = 0; i < 16; i++) {
//
//    }
//}

