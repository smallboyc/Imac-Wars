#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include <cmath>

// include
#include "Game.hpp"

// Génère la map
void Game::TowerDefense::setup_MAP()
{
    this->map.NUMBER_OF_PIXELS_IN_LINE = 10;
    this->map.schema_file = "map_schema_10x10_V4";
    this->map.get_NODES_from_ITD();
    this->map.create_GRAPH_from_NODES();
    this->map.get_SHORTER_PATH_LIST();
    this->map.generate_SCHEMA();
    this->map.get_PIXELS_from_SCHEMA();
    this->map.set_PIXELS_type();
    this->map.set_PIXELS_connected();
    this->map.get_TILES_from_PIXELS();
    this->map.render_TILES_texture();
}

// Render la map
void Game::TowerDefense::render_MAP()
{
    this->map.load_MAP();
}

// Active l'interface utilisateurs et les infos
void Game::TowerDefense::active_UI()
{
    this->ui.enabled(this->map);
}

// Récupère les données des ennemis depuis l'ITD
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
            std::vector<float> numbers;

            // on zap le premier element (type)
            iss >> level;

            float number;
            while (iss >> number)
                numbers.push_back(number);

            Enemy enemy;
            enemy.health = numbers[1] / 100;
            enemy.speed = numbers[2] / 10;
            enemy.damage = numbers[3];
            this->ENEMIES_ITD.insert({static_cast<int>(numbers[0]), enemy});
        }
    }
    inputFile.close();
}

// Récupère les données des vagues depuis l'ITD
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

// Setup la vague
void Game::TowerDefense::setup_WAVE()
{
    this->current_WAVE = this->WAVES_ITD.at(this->current_WAVE_id);
    this->WAVES_checked.push_back(this->current_WAVE_id);
    std::cout << "Vague " << this->current_WAVE_id << " => " << this->current_WAVE.number_of_ENDPOINTS << " spawns avec " << this->current_WAVE.number_of_ENEMIES << " ennemis " << std::endl;
}

// Récupère les ennemis de la vague ciblés
void Game::TowerDefense::get_ENEMIES_into_WAVE()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int ENEMY_id{0};
    for (int i{0}; i < this->current_WAVE.number_of_ENEMIES; i++)
    {
        int ENEMY_type;
        if (this->current_WAVE.ENEMIES_type.size() != 1)
        {
            auto min = std::min_element(this->current_WAVE.ENEMIES_type.begin(), this->current_WAVE.ENEMIES_type.end());
            auto max = std::max_element(this->current_WAVE.ENEMIES_type.begin(), this->current_WAVE.ENEMIES_type.end());
            ENEMY_type = *min + (std::rand() % (*max - *min + 1));
        }
        else
            ENEMY_type = this->current_WAVE.ENEMIES_type[0];

        this->current_ENEMIES.insert({ENEMY_id, this->ENEMIES_ITD.at(ENEMY_type)});
        ENEMY_id++;
    }
}

// Setup des ennemis (textures et attributs)
void Game::TowerDefense::setup_ENEMIES()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (auto &enemy : this->current_ENEMIES)
        enemy.second.set(this->map, (std::rand() % this->current_WAVE.number_of_ENDPOINTS));
}

// Update la position de l'ennemi en temps réel
void Game::TowerDefense::update_ENEMIES(const double &elapsedTime)
{
    for (auto &enemy : this->current_ENEMIES)
        enemy.second.update_state(this->map, elapsedTime);
}

// Affichage de l'ennemi
void Game::TowerDefense::render_ENEMIES()
{
    for (auto &enemy : this->current_ENEMIES)
    {
        glPushMatrix();
        if (!enemy.second.isDead)
            enemy.second.move(this->map);
        glPopMatrix();
    }
}

// Update des vagues en fonction de l'avancée du jeu
void Game::TowerDefense::update_WAVE()
{
    if (std::find(this->WAVES_checked.begin(), this->WAVES_checked.end(), this->current_WAVE_id) == this->WAVES_checked.end())
    {
        setup_WAVE();
        get_ENEMIES_into_WAVE();
        setup_ENEMIES();
    }

    // Si l'ennemi meurt, on l'enlève de notre liste dans la vague
    for (auto &enemy : this->current_ENEMIES)
        if (enemy.second.isDead)
            this->current_ENEMIES.erase(enemy.first);

    // Plus d'ennemis dans la vague actuelle ? On passe à la suivante
    if (this->current_ENEMIES.empty())
        this->current_WAVE_id++;
}
