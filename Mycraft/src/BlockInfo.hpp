//
//  BlockInfo.hpp
//  Mycraft
//
//  Created by Clapeysron on 28/12/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#ifndef BlockInfo_hpp
#define BlockInfo_hpp
#include <string>
#include "game_settings.h"
#include <map>

class BlockInfo {
public:
    BlockInfo() {};
    BlockInfo(const BlockInfo& aBlockInfo) {
        this->block_name = aBlockInfo.block_name;
        this->broke_time = aBlockInfo.broke_time;
        this->y_pos_x = aBlockInfo.y_pos_x;
        this->y_pos_y = aBlockInfo.y_pos_y;
        this->y_neg_x = aBlockInfo.y_neg_x;
        this->y_neg_y = aBlockInfo.y_neg_y;
        this->x_pos_x = aBlockInfo.x_pos_x;
        this->x_pos_y = aBlockInfo.x_pos_y;
        this->x_neg_x = aBlockInfo.x_neg_x;
        this->x_neg_y = aBlockInfo.x_neg_y;
        this->z_pos_x = aBlockInfo.z_pos_x;
        this->z_pos_y = aBlockInfo.z_pos_y;
        this->z_neg_x = aBlockInfo.z_neg_x;
        this->z_neg_y = aBlockInfo.z_neg_y;
    };
    BlockInfo(std::string block_name, float broke_time, float y_pos_x, float y_pos_y) {
        this->block_name = block_name;
        this->broke_time = broke_time;
        this->y_pos_x = this->y_neg_x = this->x_pos_x = this->x_neg_x = this->z_pos_x = this->z_neg_x = y_pos_x;
        this->y_pos_y = this->y_neg_y = this->x_pos_y = this->x_neg_y = this->z_pos_y = this->z_neg_y = y_pos_y;
    };
    BlockInfo(std::string block_name, float broke_time, float y_pos_x, float y_pos_y, float y_neg_x, float y_neg_y, float x_pos_x, float x_pos_y) {
        this->block_name = block_name;
        this->broke_time = broke_time;
        this->y_pos_x = y_pos_x;
        this->y_pos_y = y_pos_y;
        this->y_neg_x = y_neg_x;
        this->y_neg_y = y_neg_y;
        this->x_pos_x = this->x_neg_x = this->z_pos_x = this->z_neg_x = x_pos_x;
        this->x_pos_y = this->x_neg_y = this->z_pos_y = this->z_neg_y = x_pos_y;
    };
    BlockInfo(std::string block_name, float broke_time, float y_pos_x, float y_pos_y, float y_neg_x, float y_neg_y, float x_pos_x, float x_pos_y, float x_neg_x, float x_neg_y, float z_pos_x, float z_pos_y, float z_neg_x, float z_neg_y) {
        this->block_name = block_name;
        this->broke_time = broke_time;
        this->y_pos_x = y_pos_x;
        this->y_pos_y = y_pos_y;
        this->y_neg_x = y_neg_x;
        this->y_neg_y = y_neg_y;
        this->x_pos_x = x_pos_x;
        this->x_pos_y = x_pos_y;
        this->x_neg_x = x_neg_x;
        this->x_neg_y = x_neg_y;
        this->z_pos_x = z_pos_x;
        this->z_pos_y = z_pos_y;
        this->z_neg_x = z_neg_x;
        this->z_neg_y = z_neg_y;
    };
    
    bool operator<(const BlockInfo & ct) const   // 两个const是必需的。
    {
        if(block_name < ct.block_name) {
            return true;
        } else {
            return false;
        }
    }
    std::string block_name;
    float broke_time;
    float y_pos_x;
    float y_pos_y;
    float y_neg_x;
    float y_neg_y;
    float x_pos_x;
    float x_pos_y;
    float x_neg_x;
    float x_neg_y;
    float z_pos_x;
    float z_pos_y;
    float z_neg_x;
    float z_neg_y;
    float bottom_texture_x;
    float bottom_texture_y;
};

extern std::map<char, BlockInfo> BlockInfoMap;

#endif /* BlockInfo_hpp */
