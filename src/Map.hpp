#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <img/img.hpp>
#include <glad/glad.h>
#include <simpletext.h>
#include "utils.hpp"

struct Map
{
public:
    img::Image SCHEMA{img::load(make_absolute_path("images/map_schema_10x10.png", true), 3, true)};
    std::vector<Tile> TILES;
    std::vector<Pixel> PIXELS;
    std::vector<Node> NODES;
    void generate_SCHEMA(std::filesystem::path const &path);
    void draw_quad_with_texture(GLuint const &texture, Pixel const &pixel);
    void get_NODES_from_ITD(std::string const &ITD_file_name);
    void get_PIXELS_from_SCHEMA();
    void get_TILES_from_PIXELS();
    void render_TILES_texture();
    void set_TILES_as_NODES();
    void set_TILES_connected();
    void load_MAP();

    // Debug
    void display_TILES_informations();

private:
    // MAP PARAMETERS (SQUARE)
    float MAP_SIZE{1.f}; // 1.0F <=> 1000px
    float SEMI_MAP_SIZE{MAP_SIZE / 2.0f};

    // PIXEL PARAMETERS
    size_t NUMBER_OF_PIXELS_IN_LINE{SCHEMA.width()};
};
