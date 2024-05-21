#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <sstream>
#include <fstream>
#include <cmath>

// include
#include "Game.hpp"

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

void Game::TowerDefense::render_MAP()
{
    this->map.load_MAP();
}

void Game::TowerDefense::active_UI()
{
    this->ui.enabled(this->map);
}

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
            enemy.type = static_cast<int>(numbers[0]);
            enemy.health = numbers[1];
            enemy.speed = numbers[2];
            enemy.damage = numbers[3];
            this->Enemies.push_back(enemy);
        }
    }
    inputFile.close();
}

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

            Wave new_wave;
            new_wave.level = static_cast<int>(numbers[0]);
            new_wave.number_of_ENDPOINTS = numbers[1];
            new_wave.number_of_ENEMIES = numbers[2];
            for (size_t i{3}; i < numbers.size(); i++)
                new_wave.ENEMIES_type.push_back(numbers[i]);
            this->Waves.push_back(new_wave);
        }
    }
    inputFile.close();
}

void Game::TowerDefense::set_WAVE()
{
    Wave current_wave;
    for (Wave &wave : this->Waves)
    {
        if (this->current_wave_id == wave.level)
        {
            current_wave = wave;
            break;
        }
    }

    // for (int i{0}; i < current_wave.ENEMIES_number; i++)
    // {

    // }
}