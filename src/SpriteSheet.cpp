#include <iostream>
#include <unordered_map>
#include <filesystem>
#include "SpriteSheet.hpp"
#include "Map.hpp"

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
    float centerOffset = map.SEMI_MAP_SIZE - map.PIXEL_SIZE / 2; // Par défaut la case est centrée en (0,0), on doit donc l'ajuster.
    float X0 = (x - map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;
    float X1 = (x + map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;
    float Y0 = (y - map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;
    float Y1 = (y + map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2d(this->next_frame_in_LINE, this->next_frame_in_COL);
    glVertex2f(X0, Y0);

    glTexCoord2d(this->next_frame_in_LINE + this->frame_width, this->next_frame_in_COL);
    glVertex2f(X1, Y0);

    glTexCoord2d(this->next_frame_in_LINE + this->frame_width, this->next_frame_in_COL + this->frame_height);
    glVertex2f(X1, Y1);

    glTexCoord2d(this->next_frame_in_LINE, this->next_frame_in_COL + this->frame_height);
    glVertex2f(X0, Y1);

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}