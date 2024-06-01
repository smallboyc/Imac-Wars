#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

// include
#include "Game.hpp"

// Récupère les données des ennemis depuis l'ITD : this->ENEMIES_ITD
void Game::TowerDefense::get_ENEMIES_from_ITD()
{
    std::ifstream inputFile("../../data/enemy.itd");
    std::string line;

    while (getline(inputFile, line))
    {
        // Récupération des paramètres des ennemis.
        if (line.find("type") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string level;
            std::string name;
            std::vector<float> numbers;

            // on zap le premier element (type)
            iss >> level;

            // On récupère le nom de l'ennemi
            iss >> name;

            float number;
            while (iss >> number)
                numbers.push_back(number);

            Enemy enemy;
            enemy.name = name;
            enemy.health = numbers[1] / 100;
            enemy.speed = numbers[2] / 10;
            enemy.damage = numbers[3];
            this->ENEMIES_ITD.insert({static_cast<int>(numbers[0]), enemy});
        }
    }
    inputFile.close();
}

// Récupère les données des vagues depuis l'ITD : this->WAVES_ITD
void Game::TowerDefense::get_WAVES_from_ITD()
{
    std::ifstream inputFile("../../data/wave.itd");
    std::string line;

    while (getline(inputFile, line))
    {
        if (line.find("level") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string level;
            std::vector<float> numbers;

            iss >> level;

            float number;
            while (iss >> number)
                numbers.push_back(number);

            Wave wave;
            wave.number_of_ENDPOINTS = numbers[1];
            wave.number_of_ENEMIES = numbers[2];
            for (size_t i{3}; i < numbers.size(); i++)
                wave.ENEMIES_type.push_back(numbers[i]);
            this->WAVES_ITD.insert({static_cast<int>(numbers[0]), wave});
        }
    }
    inputFile.close();
}

// Récupère les données des particules depuis l'ITD : this->PARTICLES
void Game::TowerDefense::get_PARTICLES_from_ITD()
{
    std::ifstream inputFile("../../data/particle.itd");
    std::string line;

    while (getline(inputFile, line))
    {
        if (line.find("type") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string type;
            std::string key;
            float frames_in_LINE;
            float frames_in_COL;
            std::string path;

            iss >> type;
            //

            iss >> key;
            iss >> frames_in_LINE;
            iss >> frames_in_COL;
            iss >> path;

            SpriteSheet sprite;
            sprite.frames_in_LINE = frames_in_LINE;
            sprite.frames_in_COL = frames_in_COL;
            sprite.path = "images/SpriteSheets/" + path;
            this->PARTICLES.insert({key, sprite});
        }
    }
    inputFile.close();
}

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

//Récupère les informations des NODES du ITD
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