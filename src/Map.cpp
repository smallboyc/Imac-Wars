#include <iostream>
#include <vector>
#include <array>
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

// #CHECK : Vérifie si les éléments "map", "path", "in", "out", "graph", "node" sont dans l'ordre.
void Map::check_order_elements_ITD()
{
    std::array<std::string, 6> elements{"map", "path", "in", "out", "graph", "node"};
    // Construct the path to the ITD file
    std::string ITD_path = "../../data/" + this->schema_ITD_file;
    std::ifstream inputFile(ITD_path);

    if (!inputFile.is_open())
    {
        std::cerr << "ERREUR : Impossible d'ouvrir le fichier : " << ITD_path << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string line;
    std::vector<std::string> elements_to_check;
    size_t i = 0;

    while (getline(inputFile, line))
    {
        if (i >= elements.size())
            break;

        if (line.find(elements[i]) != std::string::npos)
        {
            std::istringstream iss(line);
            std::string _type;
            iss >> _type;

            elements_to_check.push_back(_type);
            i++;
        }
    }

    inputFile.close();

    for (size_t i = 0; i < elements_to_check.size(); i++)
        if (elements_to_check[i] != elements[i])
        {
            std::cerr << "ERREUR - ITD : Les paramètres ne sont pas appelés dans le bon ordre." << std::endl;
            std::exit(EXIT_FAILURE);
        }

    if (elements_to_check.size() != elements.size())
    {
        std::cerr << "ERREUR - ITD : Il manque des paramètres." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

// Récupère l'image en pixel de la map depuis l'itd
void Map::get_SCHEMA_from_ITD()
{
    std::string ITD_path{"../../data/"};
    ITD_path += this->schema_ITD_file;
    std::string IMG_path{"images/"};
    std::ifstream inputFile(ITD_path);
    std::string line;

    while (getline(inputFile, line))
    {
        // Récupération des paramètres des nodes.
        if (line.find("map") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string _type;
            std::string path;

            // on zap le premier element (node)
            iss >> _type;

            iss >> path;
            IMG_path += path;
        }
    }
    inputFile.close();

    this->SCHEMA = img::load(make_absolute_path(IMG_path, true), 3, true);
}

// Récupère le code couleur des types de pixel du ITD
Color Map::get_colors_from_ITD(std::string const &type)
{
    Color color;
    // Open the input file named "input.txt"
    std::string ITD_path = {"../../data/"};
    ITD_path += this->schema_ITD_file;
    std::ifstream inputFile(ITD_path);
    std::string line;

    while (getline(inputFile, line))
    {
        // Récupération des paramètres des nodes.
        if (line.find(type) != std::string::npos)
        {
            std::istringstream iss(line);
            std::string _type;
            std::vector<int> color_numbers;

            iss >> _type;

            int number;
            while (iss >> number)
            {
                if (number < 0 || number > 255)
                {
                    std::cerr << "ERREUR - ITD : " << type << " invalide : " << number << " n'est pas entre 0 et 255." << std::endl;
                    std::exit(EXIT_FAILURE);
                }
                color_numbers.push_back(number);
            }

            color = {color_numbers[0], color_numbers[1], color_numbers[2]};
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
    ITD_path += this->schema_ITD_file;
    std::ifstream inputFile(ITD_path);
    std::string line;

    while (getline(inputFile, line))
    {
        // Récupération des paramètres des nodes.
        if (line.find("node") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string node;
            std::vector<float> numbers;

            // on zap le premier element (node)
            iss >> node;

            float number;
            while (iss >> number)
                numbers.push_back(number);

            Node new_node;
            new_node.id = static_cast<int>(numbers[0]); // On génère l'ID
            new_node.pixel = {numbers[1], numbers[2]};  // On attribue la coordonnée du node (hérite de Pixel) // On donne la connexion auquel est lié le node
            for (size_t i{3}; i < numbers.size(); i++)
                new_node.connected_to.push_back(numbers[i]); // On ajoute tous les id des nodes connectés au node courant
            NODES.push_back(new_node);
        }
    }
    inputFile.close();
    this->NODES = NODES;
}

// 2) Créer le GRAPH depuis le tableau de nodes
void Map::create_GRAPH_from_NODES()
{
    for (Node &node : this->NODES)
        for (int connected_node_id : node.connected_to)
            for (Node &node_to_connect : this->NODES)
                if (connected_node_id == node_to_connect.id)
                {
                    float distance = (std::abs(node.pixel.x - node_to_connect.pixel.x) + std::abs(node.pixel.y - node_to_connect.pixel.y));
                    this->GRAPH.add_directed_edge(node.id, node_to_connect.id, distance);
                }
}

// 3) Détermine tous les plus courts chemins du GRAPH en les stockant dans un vector
void Map::get_SHORTER_PATH_LIST()
{
    std::vector<int> END_POINTS;
    int START_POINT{0};

    for (Node &node : this->NODES)
    {
        // On va chercher les END POINTS
        if (node.connected_to.empty())
            END_POINTS.push_back(node.id);
    }

    // Pour chaque END POINTS, on leur attribue un plus court chemin par rapport à l'entrée
    for (int end_point : END_POINTS)
    {
        std::unordered_map<int, std::pair<float, int>> DISTANCES{this->GRAPH.dijkstra(START_POINT, end_point)};
        std::vector<int> SHORTER_PATH_ID;
        std::vector<Node> SHORTER_PATH;
        auto finalEdge{DISTANCES.at(end_point)};
        // std::cout << "Distance minimale : " << finalEdge.first << std::endl;
        // std::cout << end << " -> ";
        SHORTER_PATH_ID.push_back(end_point);
        while (finalEdge.second != START_POINT)
        {
            SHORTER_PATH_ID.push_back(finalEdge.second);
            // std::cout << finalEdge.second << " -> ";
            finalEdge = DISTANCES.at(finalEdge.second);
        }
        SHORTER_PATH_ID.push_back(START_POINT);

        // Du tableau d'id, on en déduit les nodes composants le chemin le plus court dans l'ordre.
        for (int shorter_node_id : SHORTER_PATH_ID)
        {
            for (Node &node : this->NODES)
                if (node.id == shorter_node_id)
                    SHORTER_PATH.push_back(node);
        }
        this->SHORTER_PATH_LIST.push_back(SHORTER_PATH);
        // std::cout << start << std::endl;
    }
}

// 5) Récupère tous les pixels du SCHEMA dans PIXELS
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

// 6) Donne un type à tous les pixels (chemin,herbe,noeud...) grâce au couleurs de l'ITD
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
            // node.pixel.y = this->SCHEMA.width() - 1 - node.pixel.y; // Conversion pour changement de repère :  f(x,y) = (x, width() - y)
            if (pixel == node.pixel)
                pixel.is_NODE = true;
        }
    }
}

// 7) Connecte tous les pixels entre eux => Connaissance forte de l'entourage
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

// 8) Récupère la tile qui correspond au pixel (en fonction de ses informations)
void Map::get_TILES_from_PIXELS()
{
    std::vector<Tile> TILES_list;
    for (Pixel &pixel : this->PIXELS)
    {
        std::vector<std::filesystem::path> TILE_path_list;
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
        // else // Herbe
        // {
        //     TILE_path_list.push_back("images/Tiles/tile_0110.png");
        //     // set ici la végétation aléatoire
        // }

        if (!TILE_path_list.empty())
            TILES_list.push_back({pixel, {}, TILE_path_list});
    }
    this->TILES = TILES_list;
}

// 9) Rendu des textures de la map
void Map::render_TILES_texture(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    for (Tile &tile : this->TILES)
    {
        for (std::filesystem::path &path : tile.path_list)
        {
            if (!LoadedTextures.contains(path))
                LoadedTextures[path] = loadTexture(img::load(make_absolute_path(path, true), 4, true));
            tile.texture_list.push_back(LoadedTextures[path]);
        }
    }
}

// 8) Génération de la map en dessinant via 'draw_quad_with_texture'
void Map::load_MAP()
{
    for (Tile &tile : this->TILES)
        for (GLuint &texture : tile.texture_list)
            draw_quad_with_texture(texture, tile.pixel.x, tile.pixel.y, *this);
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

// Affiche tous les plus courts chemins
void Map::display_SHORTER_PATH()
{
    for (auto &shorter_path : this->SHORTER_PATH_LIST)
        for (Node &node : shorter_path)
            std::cout << node.id << " : (" << node.pixel.x << "," << node.pixel.y << ")" << std::endl;
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