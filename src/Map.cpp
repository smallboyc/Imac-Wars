#include <iostream>
#include <vector>
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

void Map::draw_quad_with_texture(GLuint textureId, Pixel pixel)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f((-SEMI_MAP_SIZE + pixel.x) / NUMBER_OF_PIXELS_IN_LINE - SEMI_MAP_SIZE, (-SEMI_MAP_SIZE + pixel.y) / NUMBER_OF_PIXELS_IN_LINE - SEMI_MAP_SIZE);

    glTexCoord2d(1, 0);
    glVertex2f((SEMI_MAP_SIZE + pixel.x) / NUMBER_OF_PIXELS_IN_LINE - SEMI_MAP_SIZE, (-SEMI_MAP_SIZE + pixel.y) / NUMBER_OF_PIXELS_IN_LINE - SEMI_MAP_SIZE);

    glTexCoord2d(1, 1);
    glVertex2f((SEMI_MAP_SIZE + pixel.x) / NUMBER_OF_PIXELS_IN_LINE - SEMI_MAP_SIZE, (SEMI_MAP_SIZE + pixel.y) / NUMBER_OF_PIXELS_IN_LINE - SEMI_MAP_SIZE);

    glTexCoord2d(0, 1);
    glVertex2f((-SEMI_MAP_SIZE + pixel.x) / NUMBER_OF_PIXELS_IN_LINE - SEMI_MAP_SIZE, (SEMI_MAP_SIZE + pixel.y) / NUMBER_OF_PIXELS_IN_LINE - SEMI_MAP_SIZE);

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

std::vector<Pixel> Map::get_map_pixels(img::Image &map_texture)
{
    std::vector<Pixel> map_pixel_colors;
    Pixel pixel;

    for (size_t i = 0; i < map_texture.data_size(); i += 3)
    {
        pixel.x = i / 3 % NUMBER_OF_PIXELS_IN_LINE;
        pixel.y = i / 3 / NUMBER_OF_PIXELS_IN_LINE;
        pixel.color = {(int)*(map_texture.data() + i), (int)*(map_texture.data() + i + 1), (int)*(map_texture.data() + i + 2)};
        map_pixel_colors.push_back(pixel);
    }
    return map_pixel_colors;
}

std::vector<Tile> Map::from_pixels_to_tiles(std::vector<Pixel> const &pixels)
{
    std::filesystem::path TILE_path;
    std::vector<Tile> TILES;
    for (Pixel pixel : pixels)
    {
        if (pixel.color == get_colors_from_itd("in"))
            TILE_path = "images/Tiles/tile_0026.png";
        else if (pixel.color == get_colors_from_itd("out"))
            TILE_path = "images/Tiles/tile_0117.png";
        else if (pixel.color == get_colors_from_itd("path"))
            TILE_path = "images/Tiles/tile_0056.png";
        else
            TILE_path = "images/Tiles/tile_0050.png";
        TILES.push_back({pixel, 0, TILE_path});
    }
    return TILES;
}
