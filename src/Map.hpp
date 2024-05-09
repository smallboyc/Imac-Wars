#pragma once
#include <iostream>
#include <img/img.hpp>
#include <glad/glad.h>
#include <simpletext.h>
#include "utils.hpp"

struct Map
{
public:
    Map() : SCHEMA(img::load(make_absolute_path("images/map_schema.png", true))) {}
    img::Image SCHEMA;         // image contenant le schema simplifié de la map (10x10px)
    std::vector<Tile> TILES;   // Contient toutes les tiles associées au schema
    std::vector<Pixel> PIXELS; // Contient tous les pixels du schema
    void generate_SCHEMA(std::filesystem::path const &path);
    void draw_quad_with_texture(GLuint const &texture, Pixel const &pixel);
    void get_PIXELS_from_SCHEMA();
    void get_TILES_from_PIXELS();
    void render_TILES_texture();
    void load_MAP();

private:
    // MAP PARAMETERS (SQUARE)
    float MAP_SIZE{1.f}; // 1.0F <=> 1000px
    float SEMI_MAP_SIZE{MAP_SIZE / 2.0f};

    // PIXEL PARAMETERS
    int NUMBER_OF_PIXELS_IN_LINE{10}; // every line
};
