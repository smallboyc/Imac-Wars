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

// MAP
void Game::TowerDefense::setup_MAP(std::string const path_itd, int const pixels_in_LINE)
{
    this->map.NUMBER_OF_PIXELS_IN_LINE = pixels_in_LINE;
    this->map.schema_ITD_file = path_itd;
    this->map.check_order_elements_ITD();
    this->map.get_SCHEMA_from_ITD();
    this->map.get_NODES_from_ITD();
    this->map.create_GRAPH_from_NODES();
    this->map.get_SHORTER_PATH_LIST();
    this->map.get_PIXELS_from_SCHEMA();
    this->map.set_PIXELS_type();
    this->map.set_PIXELS_connected();
    this->map.get_TILES_from_PIXELS();
    this->map.load_TILES_MAP(this->LoadedTextures);
}

void Game::TowerDefense::render_MAP()
{
    this->map.load_MAP();
}

// Active l'interface utilisateurs et les infos
void Game::TowerDefense::active_UI()
{
    this->ui.show_CELLS(this->map);
    this->ui.show_ENEMY_PROPERTIES(this->current_ENEMIES_in_WAVE);
}

// Récupère la current_WAVE depuis l'ITD avec un id.
void Game::TowerDefense::setup_WAVE()
{
    this->current_WAVE = this->WAVES_ITD.at(this->current_WAVE_id);
    this->WAVES_checked.push_back(this->current_WAVE_id);
    std::cout << "Vague " << this->current_WAVE_id << " => " << this->current_WAVE.number_of_ENDPOINTS << " spawns avec " << this->current_WAVE.number_of_ENEMIES << " ennemis " << std::endl;
}

// Récupère les ennemis de la vague (aléatoirement en fonction des types référencés)
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

        this->current_ENEMIES_in_WAVE.insert({ENEMY_id, this->ENEMIES_ITD.at(ENEMY_type)});
        ENEMY_id++;
    }
}

// Setup des ennemis (textures et attributs)
void Game::TowerDefense::setup_ENEMIES_in_WAVE()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (auto &enemy : this->current_ENEMIES_in_WAVE)
        enemy.second.set(this->map, (std::rand() % this->current_WAVE.number_of_ENDPOINTS), this->LoadedTextures);
}

// Update la position de l'ennemi en temps réel
void Game::TowerDefense::update_ENEMIES_in_WAVE(const double &elapsedTime, const double &currentTime)
{
    for (auto &enemy : this->current_ENEMIES_in_WAVE)
    {
        enemy.second.update_state(this->map, elapsedTime);
        if (enemy.second.is_burning)
            this->PARTICLES.at("FIRE_ORANGE").updateSpriteSheet(currentTime);
    }
}

// Affichage de l'ennemi
void Game::TowerDefense::render_ENEMIES_in_WAVE()
{
    for (auto &enemy : this->current_ENEMIES_in_WAVE)
    {
        if (!enemy.second.isDead)
        {
            glPushMatrix();
            enemy.second.move(this->map);
            glPopMatrix();

            if (enemy.second.is_burning)
            {
                glPushMatrix();
                glTranslatef(0, 0.01, 0);
                this->PARTICLES.at("FIRE_ORANGE").renderSpriteSheet(enemy.second.pos.x, enemy.second.pos.y, this->map);
                glPopMatrix();
            }
        }
    }
}

// Update des vagues en fonction de l'avancée du jeu
void Game::TowerDefense::update_WAVE()
{
    // Le jeu se termine quand on a effectué toutes les vagues de l'ITD.
    if (this->current_WAVE_id != this->WAVES_ITD.size())
    {
        if (std::find(this->WAVES_checked.begin(), this->WAVES_checked.end(), this->current_WAVE_id) == this->WAVES_checked.end())
        {
            setup_WAVE();
            get_ENEMIES_into_WAVE();
            setup_ENEMIES_in_WAVE();
        }

        // Si l'ennemi meurt, on l'enlève de notre liste dans la vague
        for (auto &enemy : this->current_ENEMIES_in_WAVE)
            if (enemy.second.isDead)
                this->current_ENEMIES_in_WAVE.erase(enemy.first);

        // Plus d'ennemis dans la vague actuelle ? On passe à la suivante
        if (this->current_ENEMIES_in_WAVE.empty())
            this->current_WAVE_id++;
    }
    else
    {
        std::cout << "FIN DU JEU" << std::endl;
        // écran de fin au lieu d'exit après.
        exit(0);
    }
}

// PARTICLE
void Game::TowerDefense::setup_PARTICLES()
{
    for (auto &particle : this->PARTICLES)
        particle.second.loadSpriteSheet(this->LoadedTextures);
}