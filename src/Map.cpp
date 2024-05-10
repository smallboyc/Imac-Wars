#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <sstream>
#include <fstream>

#include "simpletext.h"
#include "Map.hpp"
#include "App.hpp"
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

    for (size_t i = 0; i < this->SCHEMA.data_size(); i += SCHEMA.channels_count())
    {
        pixel.x = i / SCHEMA.channels_count() % this->NUMBER_OF_PIXELS_IN_LINE;
        pixel.y = i / SCHEMA.channels_count() / this->NUMBER_OF_PIXELS_IN_LINE;
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
        Connections NEIGHBOUR{pixel.PIXEL_connection};
        if (pixel.color == get_colors_from_itd("in")) // Point d'entrée
            TILE_path = "images/Tiles/tile_0026.png";
        else if (pixel.color == get_colors_from_itd("out")) // Point de sortie
            TILE_path = "images/Tiles/tile_0117.png";
        else if (pixel.color == get_colors_from_itd("path")) // Point de chemin => route OU virage
        {
            if (pixel.is_NODE) // virage
            {
                if (NEIGHBOUR.top->is_VOID && NEIGHBOUR.right->is_VOID)
                    TILE_path = "images/Tiles/tile_0075.png";
                else if (NEIGHBOUR.top->is_VOID && NEIGHBOUR.left->is_VOID)
                    TILE_path = "images/Tiles/tile_0073.png";
                else if (NEIGHBOUR.bottom->is_VOID && NEIGHBOUR.right->is_VOID)
                    TILE_path = "images/Tiles/tile_0075_bis.png";
                else if (NEIGHBOUR.bottom->is_VOID && NEIGHBOUR.left->is_VOID)
                    TILE_path = "images/Tiles/tile_0073_bis.png";
            }
            else // route
            {
                if (NEIGHBOUR.top->is_VOID && NEIGHBOUR.bottom->is_VOID)
                    TILE_path = "images/Tiles/tile_0098.png";
                else
                    TILE_path = "images/Tiles/tile_0087.png";
            }
        }
        else // Herbe
        {
            TILE_path = "images/Tiles/tile_0050.png";
        }
        TILES.push_back({pixel, 0, TILE_path});
    }
    this->TILES = TILES;
}

void Map::render_TILES_texture()
{
    for (Tile &tile : this->TILES)
        tile.texture = loadTexture(img::load(make_absolute_path(tile.path, true), 4, true));
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

void Map::get_NODES_from_ITD(std::string const &ITD_file_name)
{
    std::vector<Node> NODES;
    std::string ITD_path = {"../../data/"};
    ITD_path += ITD_file_name;
    std::ifstream inputFile(ITD_path);
    std::string line;

    while (getline(inputFile, line))
    {
        // Récupération des paramètres des nodes.
        if (line.find("node") != std::string::npos)
        {
            std::string node_str{""};
            for (char c : line)
                if (isdigit(c))
                    node_str += c;

            Node new_node{node_str[0] - '0'};                        // On génère l'ID
            new_node.pixel = {node_str[1] - '0', node_str[2] - '0'}; // On attribue la coordonnée du node (hérite de Pixel)
            new_node.connected_to = node_str[3] - '0';               // On donne la connexion auquel est lié le node
            // Pour le Node color ?
            NODES.push_back(new_node);
        }
    }
    inputFile.close();

    this->NODES = NODES;
}

void Map::set_PIXELS_TYPE()
{
    Color VOID{0, 0, 0};
    for (Pixel &pixel : this->PIXELS)
    {
        if (pixel.color == VOID)
            pixel.is_VOID = true;

        for (Node node : this->NODES)
        {
            node.pixel.y = this->SCHEMA.width() - 1 - node.pixel.y; // Conversion pour changement de repère :  f(x,y) = (x, width() - y)
            if (pixel == node.pixel)
                pixel.is_NODE = true;
        }
    }
}

void Map::set_PIXELS_connected()
{
    for (Pixel &pixel : this->PIXELS)
    {
        pixel.PIXEL_connection = {};

        for (Pixel &pixel_neighbour : this->PIXELS)
        {
            if (&pixel_neighbour == &pixel)
                continue;

            Pixel top{pixel.x, pixel.y + 1};
            Pixel bottom{pixel.x, pixel.y - 1};
            Pixel right{pixel.x + 1, pixel.y};
            Pixel left{pixel.x - 1, pixel.y};

            if (pixel_neighbour == top)
                pixel.PIXEL_connection.top = &pixel_neighbour;
            else if (pixel_neighbour == bottom)
                pixel.PIXEL_connection.bottom = &pixel_neighbour;
            else if (pixel_neighbour == right)
                pixel.PIXEL_connection.right = &pixel_neighbour;
            else if (pixel_neighbour == left)
                pixel.PIXEL_connection.left = &pixel_neighbour;
        }
    }
}

// Debug

void Map::display_PIXELS_informations()
{
    for (Pixel pixel : this->PIXELS)
    {
        std::cout << "(" << pixel.x << "," << pixel.y << ") -> (" << pixel.color.r << "," << pixel.color.g << "," << pixel.color.b << ")";
        if (pixel.is_NODE)
            std::cout << " = NODE";
        if (pixel.is_VOID)
            std::cout << " -> VOID ";

        Connections &NEIGHBOUR{pixel.PIXEL_connection};
        std::cout << " & connected to => ";
        if (NEIGHBOUR.top != nullptr)
            std::cout << "TOP : " << " (" << NEIGHBOUR.top->x << ":" << NEIGHBOUR.top->y << ") ";
        else
            std::cout << "TOP : " << " ? ";
        if (NEIGHBOUR.bottom != nullptr)
            std::cout << "BOTTOM : " << " (" << NEIGHBOUR.bottom->x << ":" << NEIGHBOUR.bottom->y << ") ";
        else
            std::cout << "BOTTOM : " << " ? ";
        if (NEIGHBOUR.left != nullptr)
            std::cout << "LEFT : " << " (" << NEIGHBOUR.left->x << ":" << NEIGHBOUR.left->y << ") ";
        else
            std::cout << "LEFT : " << " ? ";
        if (NEIGHBOUR.right != nullptr)
            std::cout << "RIGHT : " << " (" << NEIGHBOUR.right->x << ":" << NEIGHBOUR.right->y << ") ";
        else
            std::cout << "RIGHT : " << " ? ";

        std::cout << std::endl;
    }
}
