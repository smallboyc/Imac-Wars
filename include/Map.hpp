#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <img/img.hpp>
#include <glad/glad.h>
#include <simpletext.h>
#include "utils.hpp"
#include "Graph.hpp"

struct Map
{
    size_t NUMBER_OF_PIXELS_IN_LINE{15};
    std::string schema_file{"map_schema_15x15"};
    img::Image SCHEMA{img::load(make_absolute_path("images/map_schema_15x15.png", true), 3, true)};
    std::vector<Tile> TILES;
    std::vector<Pixel> PIXELS;
    std::vector<Node> NODES;
    Graph::WeightedGraph GRAPH;
    std::vector<std::vector<Node>> SHORTER_PATH_LIST;

    Color get_colors_from_ITD(std::string const &type);
    void get_NODES_from_ITD();
    void create_GRAPH_from_NODES();
    void get_SHORTER_PATH_LIST();
    void generate_SCHEMA();
    void get_PIXELS_from_SCHEMA();
    void set_PIXELS_type();
    void set_PIXELS_connected();
    void get_TILES_from_PIXELS();
    void render_TILES_texture();
    // GRAPH
    void load_MAP();

    // Debug
    void display_PIXELS_informations();
    void display_SHORTER_PATH();

    // MAP PARAMETERS (SQUARE)
    float MAP_SIZE{1.f}; // 1.0F <=> 1000px
    float SEMI_MAP_SIZE{MAP_SIZE / 2.0f};
};
