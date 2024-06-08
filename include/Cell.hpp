#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>

struct Color
{
    int r{0};
    int g{0};
    int b{0};
};

struct Pixel;

struct Connections
{
    Pixel *top;
    Pixel *bottom;
    Pixel *right;
    Pixel *left;
};

struct Pixel
{
    float x{0};
    float y{0};
    Color color;
    // Type de pixel
    bool is_NODE{false};
    bool is_VOID{false};
    bool is_PATH{false};
    bool is_START_POINT{false};
    bool is_END_POINT{false};
    bool is_TOWER{false};
    // Situation
    bool on_Mouse_Over{false};

    Connections PIXEL_connection;
};

struct Node
{
    int id{0};
    Pixel pixel;
    std::vector<int> connected_to;
};

struct Tile
{
    Pixel pixel;
    std::vector<GLuint> texture_list;
    std::vector<std::filesystem::path> path_list;
};