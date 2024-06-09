#include <iostream>
#include <unordered_map>
#include <filesystem>
#include "SpriteSheet.hpp"
#include "Map.hpp"
#include "Draw.hpp"

void SpriteSheet::loadSpriteSheet(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    this->texture = LoadedTextures[this->path];
    this->frame_width = 1 / this->frames_in_LINE;
    this->frame_height = 1 / this->frames_in_COL;
    this->lastUpdateTime = 0.0;
}

void SpriteSheet::updateSpriteSheet(const double &currentTime)
{
    if (currentTime - this->lastUpdateTime >= this->frame_rate)
    {
        this->lastUpdateTime = currentTime;

        if (this->next_frame_in_LINE < 1 - this->frame_width)
            this->next_frame_in_LINE += this->frame_width;
        else
        {
            this->next_frame_in_LINE = 0;
            if (this->next_frame_in_COL < 1 - this->next_frame_in_COL)
                this->next_frame_in_COL += this->frame_height;
            else
                this->next_frame_in_COL = 0;
        }
    }
}

void SpriteSheet::renderSpriteSheet(float const &x, float const &y, Map &map)
{
    draw_spriteSheet(*this, x, y, map);
}