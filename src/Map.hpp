#pragma once
#include <iostream>
#include <img/img.hpp>
#include <glad/glad.h>
#include <simpletext.h>
#include "utils.hpp"

// Tout ce qui concerne la map, l'affichage et son évolution ici :

struct Map
{
public:
    float MAP_SIZE{1.f}; // 1.0F <=> 1000px
    GLuint map_texture_ID;
    void draw_map();
    void grid_map_show(); // dessine une grille sur la map.
    std::vector<Pixel> get_map_data(img::Image &map_texture);

    // void game_elements_show(//tower, //ennemies, //defense);

private:
    // MAP PARAMETERS (SQUARE)
    float SEMI_MAP_SIZE{MAP_SIZE / 2.0f};

    // PIXEL PARAMETERS
    int NUMBER_OF_PIXELS_IN_LINE{10}; // every line
    float PIXEL_SIZE{MAP_SIZE / NUMBER_OF_PIXELS_IN_LINE};
    float SEMI_PIXEL_SIZE{PIXEL_SIZE / 2.0f};

    // ELEMENT IN GAME
};
