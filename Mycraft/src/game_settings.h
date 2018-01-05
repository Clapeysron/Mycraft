//
//  game_settings.h
//  Mycraft
//
//  Created by Clapeysron on 14/11/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#ifndef game_settings_h
#define game_settings_h

#define SCREEN_WIDTH 2560
#define SCREEN_HEIGHT 1440

#define QUAD_SIZE 60
#define QUAD_FACE_SIZE 8
#define VERTEX_SIZE 10
#define FACE_TYPE_NUM 8

//transparent material or model (MSB = 1)
//trans cube 0x1000xxxx
#define LEAF 0x80
#define GLASS 0x81
#define CLOUD 0x82

//non-trans crossing faces 0x11xx xxxx
#define GRASS 0xc0
#define FLOWER 0xc1
#define TORCH 0xc2

//trans cube 0x1111xxxx
#define AIR 0xf0
#define WATER 0xf1
#define GLASS_RED 0xf2
#define GLASS_GREEN 0xf3
#define GLASS_BLUE 0xf4

//non-trans and non-filled material (MSB = 0)
#define BOUND 0x00
#define ROCK 0x01
#define SOIL 0x02
#define GRASSLAND 0x03
#define TRUNK 0x04
#define BASE_ROCK 0x05
#define GLOWSTONE 0x06
#define WOOD 0x07
#define RED_WOOD 0x08
#define TINT_WOOD 0x09
#define DARK_WOOD 0x0A
#define BRICK 0x0B
#define SAND 0x0C
#define COAL_ORE 0x0D
#define GOLD_ORE 0x0E
#define IRON_ORE 0x0F
#define DIAMAND_ORE 0x10
#define EMERALD_ORE 0x11
#define REDSTONE_ORE 0x12
#define TOOLBOX 0x13
#define SMELTER 0x14
#define WATERMELON 0x15
#define PUMPKIN 0x16
#define WHITE_WOOL 0x17
#define TNT 0x18
#define COBBLESTONE 0x19
#define MOSSY_COBBLESTONE 0x1A
#define STONE_BRICK 0x1B
#define QUARTZ 0x1C
#define GOLD 0x1D

#define POINT_BRIGHTNESS 0x0f

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
#define XCENTER 0x06
#define ZCENTER 0x07
//vector reverse size
#define VECTOR_OFFSET 24576

//visible region radius
#define RADIUS 7

#define GRAVITY 9.8
#define JUMP_V 3.2
#define MAX_WATER_V 3.2
#define FLOAT_UP_V 2.5

//steve radius
#define STEVE_RADIUS 0.35
#define STEVE_HEIGHT 1.8
#define STEVE_EYE_HEIGHT 1.6
#define STEVE_FETCH_RADIUS 8

//texture offset
#define TEXTURE_MIN 0.001f
#define TEXTURE_MAX 0.099f
#define GRASS_MIN 0.0f
#define GRASS_MAX 1.0f

//texture place
#define ROCK_X 0.3f
#define ROCK_Y 0.0f
#define SOIL_X 0.2f
#define SOIL_Y 0.0f
#define GRASSLAND_TOP_X 0.0f
#define GRASSLAND_TOP_Y 0.0f
#define GRASSLAND_SIDE_X 0.1f
#define GRASSLAND_SIDE_Y 0.0f
#define WATER_X 0.9f
#define WATER_Y 0.1f
#define DIAMAND_ORE_X 0.1f
#define DIAMAND_ORE_Y 0.2f
#define GRASS_X 0.0f
#define GRASS_Y 0.4f
#define BASE_ROCK_X 0.4f
#define BASE_ROCK_Y 0.0f
#define LEAF_X 0.4f
#define LEAF_Y 0.3f
#define TRUNK_TOP_X 0.2f
#define TRUNK_TOP_Y 0.1f
#define TRUNK_SIDE_X 0.3f
#define TRUNK_SIDE_Y 0.1f
#define TORCH_X 0.6f
#define TORCH_Y 0.5f
#define GLOWSTONE_X 0.7f
#define GLOWSTONE_Y 0.0f
#define WOOD_X 0.1f
#define WOOD_Y 0.3f
#define RED_WOOD_X 0.0f
#define RED_WOOD_Y 0.3f
#define TINT_WOOD_X 0.2f
#define TINT_WOOD_Y 0.3f
#define DARK_WOOD_X 0.3f
#define DARK_WOOD_Y 0.3f
#define GLASS_X 0.0f
#define GLASS_Y 0.7f

#define BRICK_X 0.5f
#define BRICK_Y 0.0f
#define SAND_X 0.6f
#define SAND_Y 0.0f
#define COAL_ORE_X 0.0f
#define COAL_ORE_Y 0.2f
#define GOLD_ORE_X 0.2f
#define GOLD_ORE_Y 0.2f
#define IRON_ORE_X 0.3f
#define IRON_ORE_Y 0.2f
#define EMERALD_ORE_X 0.4f
#define EMERALD_ORE_Y 0.2f
#define REDSTONE_ORE_X 0.5f
#define REDSTONE_ORE_Y 0.2f
#define TOOLBOX_TOP_X 0.2f
#define TOOLBOX_TOP_Y 0.5f
#define TOOLBOX_SIDE1_X 0.1f
#define TOOLBOX_SIDE1_Y 0.5f
#define TOOLBOX_SIDE2_X 0.0f
#define TOOLBOX_SIDE2_Y 0.5f
#define SMELTER_TOP_X 0.5f
#define SMELTER_TOP_Y 0.5f
#define SMELTER_SIDE1_X 0.3f
#define SMELTER_SIDE1_Y 0.5f
#define SMELTER_SIDE2_X 0.4f
#define SMELTER_SIDE2_Y 0.5f
#define WATERMELON_TOP_X 0.4f
#define WATERMELON_TOP_Y 0.6f
#define WATERMELON_SIDE_X 0.3f
#define WATERMELON_SIDE_Y 0.6f
#define PUMPKIN_TOP_X 0.6f
#define PUMPKIN_TOP_Y 0.6f
#define PUMPKIN_SIDE_X 0.5f
#define PUMPKIN_SIDE_Y 0.6f
#define WHITE_WOOL_X 0.0f
#define WHITE_WOOL_Y 0.8f
#define TNT_TOP_X 0.7f
#define TNT_TOP_Y 0.3f
#define TNT_BOTTOM_X 0.5f
#define TNT_BOTTOM_Y 0.3f
#define TNT_SIDE_X 0.6f
#define TNT_SIDE_Y 0.3f
#define COBBLESTONE_X 0.6f
#define COBBLESTONE_Y 0.2f
#define MOSSY_COBBLESTONE_X 0.7f
#define MOSSY_COBBLESTONE_Y 0.2f
#define STONE_BRICK_X 0.8f
#define STONE_BRICK_Y 0.2f
#define QUARTZ_TOP_X 0.8f
#define QUARTZ_TOP_Y 0.3f
#define QUARTZ_SIDE_X 0.9f
#define QUARTZ_SIDE_Y 0.3f
#define GOLD_X 0.9f
#define GOLD_Y 0.2f
#define CLOUD_X 0.9f
#define CLOUD_Y 0.6f

// SEA_LEVEL
#define SEA_LEVEL 115

#define VERTEX1_OFFSET 14
#define VERTEX2_OFFSET 12
#define VERTEX3_OFFSET 10
#define VERTEX4_OFFSET 8
#define VERTEX5_OFFSET 6
#define VERTEX6_OFFSET 4
#define VERTEX7_OFFSET 2
#define VERTEX8_OFFSET 0

// Water offset with the aside block
#define WATER_OFFSET 0.85
// Tree Density
#define TREE_DENSITY 500
// Sun rise time
#define SUNRISE_TIME 6

#define CORNER_SHADOW 1.0f
#define POINT_LIGHT 0.0625f

#endif /* game_settings_h */
