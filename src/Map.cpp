#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <sstream>
#include <fstream>
#include <cmath>

#include "simpletext.h"
#include "Map.hpp"
#include "App.hpp"
#include "utils.hpp"
#include "GLHelpers.hpp"

// Dessine la TILE à la coordonnée du pixel associé.
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

// Récupère le code couleur des types de pixel du ITD
Color Map::get_colors_from_ITD(std::string const &type)
{
    Color color;
    // Open the input file named "input.txt"
    std::string ITD_path = {"../../data/"};
    ITD_path += this->schema_file + ".itd";
    std::ifstream inputFile(ITD_path);
    std::vector<std::string> color_str_array{};
    std::string line;

    while (getline(inputFile, line))
    {
        // Récupération des paramètres des nodes.
        if (line.find(type) != std::string::npos)
        {
            std::string color_str{};

            for (size_t i{1}; i < line.size(); i++)
            {
                if (isdigit(line[i]) || line[i] == ' ')
                    color_str += line[i];
            }

            std::istringstream iss(color_str);
            std::vector<int> pixel_color;
            std::string token;

            while (std::getline(iss, token, ' '))
            {
                if (!token.empty())
                    pixel_color.push_back(std::stoi(token));
            }
            color = {pixel_color[0], pixel_color[1], pixel_color[2]};
        }
    }
    inputFile.close();

    return color;
}

// 1) Récupère les informations des NODES du ITD
void Map::get_NODES_from_ITD()
{
    std::vector<Node> NODES;
    std::string ITD_path{"../../data/"};
    ITD_path += this->schema_file + ".itd";
    std::ifstream inputFile(ITD_path);
    std::string line;

    while (getline(inputFile, line))
    {
        // Récupération des paramètres des nodes.
        if (line.find("node") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string node;
            std::vector<int> numbers;

            // on zap le premier element (node)
            iss >> node;

            int number;
            while (iss >> number)
                numbers.push_back(number);

            Node new_node;
            new_node.id = numbers[0];                  // On génère l'ID
            new_node.pixel = {numbers[1], numbers[2]}; // On attribue la coordonnée du node (hérite de Pixel) // On donne la connexion auquel est lié le node
            for (size_t i{3}; i < numbers.size(); i++)
                new_node.connected_to.push_back(numbers[i]); // On ajoute tous les id des nodes connectés au node courant
            NODES.push_back(new_node);
        }
    }
    inputFile.close();
    this->NODES = NODES;
}

void Map::create_GRAPH_from_NODES()
{
    for (Node &node : this->NODES)
        for (int connected_node_id : node.connected_to)
            for (Node &node_to_connect : this->NODES)
                if (connected_node_id == node_to_connect.id)
                {
                    float distance{(std::abs(node.pixel.x - node_to_connect.pixel.x) + std::abs(node.pixel.y - node_to_connect.pixel.y))};
                    this->GRAPH.add_directed_edge(node.id, node_to_connect.id, distance);
                }
}

// 2) Génère le SCHEMA référencé
void Map::generate_SCHEMA()
{
    std::string IMG_path{"images/"};
    IMG_path += this->schema_file + ".png";
    this->SCHEMA = img::load(make_absolute_path(IMG_path, true), 3, true);
}

// 3) Récupère tous les pixels du SCHEMA dans PIXELS
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

// 4) Donne un type à tous les pixels (chemin,herbe,noeud...) grâce au couleurs de l'ITD
void Map::set_PIXELS_type()
{
    for (Pixel &pixel : this->PIXELS)
    {
        if (pixel.color == get_colors_from_ITD("in"))
        {
            pixel.is_START_POINT = true;
            pixel.is_PATH = true;
        }
        else if (pixel.color == get_colors_from_ITD("out"))
        {
            pixel.is_END_POINT = true;
            pixel.is_PATH = true;
        }
        else if (pixel.color == get_colors_from_ITD("path"))
            pixel.is_PATH = true;
        else
            pixel.is_VOID = true;

        for (Node node : this->NODES)
        {
            node.pixel.y = this->SCHEMA.width() - 1 - node.pixel.y; // Conversion pour changement de repère :  f(x,y) = (x, width() - y)
            if (pixel == node.pixel)
                pixel.is_NODE = true;
        }
    }
}

// 5) Connecte tous les pixels entre eux => Connaissance forte de l'entourage
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
            else if (pixel_neighbour == left)
                pixel.PIXEL_connection.left = &pixel_neighbour;
            else if (pixel_neighbour == right)
                pixel.PIXEL_connection.right = &pixel_neighbour;
        }
    }
}

// 6) Récupère la tile qui correspond au pixel (en fonction de ses informations)
void Map::get_TILES_from_PIXELS()
{
    std::vector<Tile> TILES_list;
    std::vector<std::filesystem::path> TILE_path_list;
    for (Pixel pixel : this->PIXELS)
    {
        Connections &NEIGHBOUR{pixel.PIXEL_connection};
        if (pixel.is_START_POINT)
        {
            // Point d'entrée
            set_IN_OUT_orientation_texture(NEIGHBOUR, TILE_path_list);
            TILE_path_list.push_back("images/Tiles/tile_0026.png");
        }
        else if (pixel.is_END_POINT)
        {
            // Point de sortie
            set_IN_OUT_orientation_texture(NEIGHBOUR, TILE_path_list);
            TILE_path_list.push_back("images/Tiles/tile_0017.png");
        }
        else if (pixel.is_PATH) // Point de chemin => route OU virage
        {
            if (pixel.is_NODE) // Virage
                set_NODE_orientation_texture(NEIGHBOUR, TILE_path_list);
            else // Route
                set_PATH_orientation_texture(NEIGHBOUR, TILE_path_list);
        }
        else // Herbe
        {
            TILE_path_list.push_back("images/Tiles/tile_0050.png");
            // set ici la végétation aléatoire
        }
        TILES_list.push_back({pixel, {}, TILE_path_list});
    }
    this->TILES = TILES_list;
}

// 7) Rendu des textures de la map
void Map::render_TILES_texture()
{
    for (Tile &tile : this->TILES)
        for (std::filesystem::path &path : tile.path_list)
            tile.texture_list.push_back(loadTexture(img::load(make_absolute_path(path, true), 4, true)));
}

// 8) Génération de la map en dessinant via 'draw_quad_with_texture'
void Map::load_MAP()
{
    for (Tile &tile : this->TILES)
        for (GLuint &texture : tile.texture_list)
            this->draw_quad_with_texture(texture, tile.pixel);
}

// DEBUUUGGGG

// Informations de tous les pixels dans PIXELS
void Map::display_PIXELS_informations()
{
    for (Pixel pixel : this->PIXELS)
    {
        std::cout << "(" << pixel.x << "," << pixel.y << ") -> (" << pixel.color.r << "," << pixel.color.g << "," << pixel.color.b << ")";
        if (pixel.is_NODE)
            std::cout << " = NODE";
        if (pixel.is_START_POINT)
            std::cout << " + START ";
        if (pixel.is_END_POINT)
            std::cout << " + END ";
        if (pixel.is_PATH)
            std::cout << " -> PATH ";
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

// BONUS!! (Pas terminé)
// HARDCORE!! Remplace get_NODES_from_ITD car récupère les nodes directement sur le schéma et les ajoute dans NODS
// void Map::get_NODES_from_PIXELS_AUTO()
// {
//     Pixel START_PIXEL;
//     int id{0};
//     // 1) On trouve un START_POINT ou un END_POINT.
//     for (Pixel &pixel : this->PIXELS)
//     {
//         if (pixel.is_START_POINT || pixel.is_END_POINT)
//         {
//             START_PIXEL = pixel;
//             Node new_node{id, {START_PIXEL.x, START_PIXEL.y}};
//             this->NODES.push_back(new_node);
//             id++;
//             break;
//         }
//     }

//     // Booléen utile pour ne pas revenir sur nos pas.
//     bool go_top{true};
//     bool go_bottom{true};
//     bool go_left{true};
//     bool go_right{true};

//     // REF_PIXEL va varier sur tous les chemins pour atteindre un noeud
//     Pixel &REF_PIXEL = START_PIXEL;
//     // Tant que je n'ai pas récupéré tous mes noeuds, je continue
//     while (this->NODES.size() < 8)
//     {

//         if (go_left) // Si je peux aller à gauche
//         {
//             while (REF_PIXEL.PIXEL_connection.left->is_PATH)
//             {
//                 // On regarde si on vient pas déjà de la gauche

//                 REF_PIXEL = *REF_PIXEL.PIXEL_connection.left;
//                 if (REF_PIXEL.PIXEL_connection.left->is_VOID) // Si à gauche, il y a rien, on est au bout, donc c'est un noeud
//                 {
//                     Node new_node{id, {REF_PIXEL.x, REF_PIXEL.y, REF_PIXEL.color}};
//                     this->NODES.push_back(new_node);
//                     id++;
//                     // On vient de la droite car on est venu à gauche
//                     go_right = false;
//                     go_left = true;
//                     go_top = true;
//                     go_bottom = true;
//                 }
//             }
//         }

//         if (go_top) // Si je peux aller en haut
//         {
//             while (REF_PIXEL.PIXEL_connection.top->is_PATH)
//             {
//                 REF_PIXEL = *REF_PIXEL.PIXEL_connection.top;
//                 if (REF_PIXEL.PIXEL_connection.top->is_VOID)
//                 {
//                     Node new_node{id, {REF_PIXEL.x, REF_PIXEL.y, REF_PIXEL.color}};
//                     this->NODES.push_back(new_node);
//                     id++;
//                     // On vient d'en bas car on est venu en haut
//                     go_bottom = false;
//                     go_left = true;
//                     go_right = true;
//                     go_top = true;
//                 }
//             }
//         }
//         if (go_right) // Si je peux aller à droite
//         {
//             while (REF_PIXEL.PIXEL_connection.right->is_PATH)
//             {
//                 REF_PIXEL = *REF_PIXEL.PIXEL_connection.right;
//                 if (REF_PIXEL.PIXEL_connection.right->is_VOID)
//                 {
//                     Node new_node{id, {REF_PIXEL.x, REF_PIXEL.y, REF_PIXEL.color}};
//                     this->NODES.push_back(new_node);
//                     id++;
//                     // On vient de la gauche car on est venu à droite
//                     go_left = false;
//                     go_right = true;
//                     go_top = true;
//                     go_bottom = true;
//                 }
//             }
//         }

//         if (go_bottom) // Si je peux aller en bas
//         {
//             while (REF_PIXEL.PIXEL_connection.bottom->is_PATH)
//             {
//                 REF_PIXEL = *REF_PIXEL.PIXEL_connection.bottom;
//                 if (REF_PIXEL.PIXEL_connection.bottom->is_VOID)
//                 {
//                     Node new_node{id, {REF_PIXEL.x, REF_PIXEL.y, REF_PIXEL.color}};
//                     this->NODES.push_back(new_node);
//                     id++;
//                     // On vient d'en haut car on est venu en bas
//                     go_top = false;
//                     go_bottom = true;
//                     go_left = true;
//                     go_right = true;
//                 }
//             }
//         }
//     }

//     for (Pixel &pixel : this->PIXELS)
//         for (Node node : this->NODES)
//             if (pixel == node.pixel)
//                 pixel.is_NODE = true;
// }