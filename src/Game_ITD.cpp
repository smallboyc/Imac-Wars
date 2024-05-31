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