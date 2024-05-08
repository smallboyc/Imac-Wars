#include <iostream>
#include "Map.hpp"
#include "App.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include <sstream>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"

void Map::draw_map()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, map_texture_ID);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(-SEMI_MAP_SIZE, -SEMI_MAP_SIZE);

    glTexCoord2d(1, 0);
    glVertex2f(SEMI_MAP_SIZE, -SEMI_MAP_SIZE);

    glTexCoord2d(1, 1);
    glVertex2f(SEMI_MAP_SIZE, SEMI_MAP_SIZE);

    glTexCoord2d(0, 1);
    glVertex2f(-SEMI_MAP_SIZE, SEMI_MAP_SIZE);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void Map::grid_map_show()
{
    // Draw pixels grid
    for (float i{-SEMI_MAP_SIZE}; i < SEMI_MAP_SIZE - PIXEL_SIZE; i += PIXEL_SIZE)
    {
        for (float j{-SEMI_MAP_SIZE}; j < SEMI_MAP_SIZE - PIXEL_SIZE; j += PIXEL_SIZE)
        {
            glColor3f(1.0f, 0.0f, 1.0f);
            glBegin(GL_LINE_LOOP);
            glLineWidth(5.0f);
            glVertex2f(i, j);
            glVertex2f(i + PIXEL_SIZE, j);
            glVertex2f(i + PIXEL_SIZE, j + PIXEL_SIZE);
            glVertex2f(i, j + PIXEL_SIZE);
            glEnd();
        }
    }
}

void Map::set_map_size(float size)
{
    MAP_SIZE = size;
    SEMI_MAP_SIZE = MAP_SIZE / 2.0f;
    PIXEL_SIZE = MAP_SIZE / NUMBER_OF_PIXELS_IN_LINE;
    SEMI_PIXEL_SIZE = PIXEL_SIZE / 2.0f;
}

void Map::parameters_map_show()
{
}
