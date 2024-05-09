#include <iostream>
#include <vector>
#include "Map.hpp"
#include "App.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include <sstream>
#include <fstream>
#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"

void Map::generate_SCHEMA(std::filesystem::path const &path)
{
    this->SCHEMA = img::load(make_absolute_path(path, true), 3, true);
}

void Map::get_PIXELS_from_SCHEMA()
{
    std::vector<Pixel> PIXELS;
    Pixel pixel;

    for (size_t i = 0; i < this->SCHEMA.data_size(); i += 3)
    {
        pixel.x = i / 3 % this->NUMBER_OF_PIXELS_IN_LINE;
        pixel.y = i / 3 / this->NUMBER_OF_PIXELS_IN_LINE;
        pixel.color = {(int)*(this->SCHEMA.data() + i), (int)*(this->SCHEMA.data() + i + 1), (int)*(this->SCHEMA.data() + i + 2)};
        PIXELS.push_back(pixel);
    }
    this->PIXELS = PIXELS;
}

void Map::get_TILES_from_PIXELS()
{
    std::filesystem::path TILE_path;
    std::vector<Tile> TILES;
    for (Pixel pixel : this->PIXELS)
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
    this->TILES = TILES;
}

void Map::render_TILES_texture()
{
    for (Tile &tile : this->TILES)
        tile.texture = loadTexture(img::load(make_absolute_path(tile.path, true), 3, true));
}

void Map::load_MAP()
{
    for (Tile &tile : this->TILES)
        this->draw_quad_with_texture(tile.texture, tile.pixel);
}

void Map::draw_quad_with_texture(GLuint const &texture, Pixel const &pixel)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f((-this->SEMI_MAP_SIZE + pixel.x) / this->NUMBER_OF_PIXELS_IN_LINE - this->SEMI_MAP_SIZE, (-this->SEMI_MAP_SIZE + pixel.y) / this->NUMBER_OF_PIXELS_IN_LINE - this->SEMI_MAP_SIZE);

    glTexCoord2d(1, 0);
    glVertex2f((this->SEMI_MAP_SIZE + pixel.x) / this->NUMBER_OF_PIXELS_IN_LINE - this->SEMI_MAP_SIZE, (-this->SEMI_MAP_SIZE + pixel.y) / this->NUMBER_OF_PIXELS_IN_LINE - this->SEMI_MAP_SIZE);

    glTexCoord2d(1, 1);
    glVertex2f((this->SEMI_MAP_SIZE + pixel.x) / this->NUMBER_OF_PIXELS_IN_LINE - this->SEMI_MAP_SIZE, (this->SEMI_MAP_SIZE + pixel.y) / this->NUMBER_OF_PIXELS_IN_LINE - this->SEMI_MAP_SIZE);

    glTexCoord2d(0, 1);
    glVertex2f((-this->SEMI_MAP_SIZE + pixel.x) / this->NUMBER_OF_PIXELS_IN_LINE - this->SEMI_MAP_SIZE, (this->SEMI_MAP_SIZE + pixel.y) / this->NUMBER_OF_PIXELS_IN_LINE - this->SEMI_MAP_SIZE);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}
