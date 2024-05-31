#pragma once
#include <vector>
#include <string>
#include <img/img.hpp>
#include "GLHelpers.hpp"

struct SpriteSheet
{
    GLuint texture;
    std::filesystem::path path;
    float frames_in_LINE;
    float frames_in_COL;
    float frame_width;
    float frame_height;
    double lastUpdateTime;
    float next_frame_in_LINE{0};
    float next_frame_in_COL{0};
    void loadSpriteSheet(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures);
    void updateSpriteSheet(const double &currentTime);
    void renderSpriteSheet(float const &x, float const &y, Map &map);
};
