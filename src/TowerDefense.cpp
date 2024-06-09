#include <iostream>
#include <filesystem>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include <cmath>

#include "TowerDefense.hpp"

void TowerDefense::Load_All_Textures()
{
    std::string path = "../../images/textures";

    for (const auto &folder : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_directory(folder.path()))
        {
            for (const auto &texture : std::filesystem::directory_iterator(folder.path()))
            {
                if (std::filesystem::is_regular_file(texture) &&
                    (texture.path().extension() == ".png" || texture.path().extension() == ".jpg"))
                {
                    std::string texture_path = std::filesystem::relative(texture.path(), "../../").string();
                    this->LoadedTextures[texture_path] = loadTexture(img::load(make_absolute_path(texture_path, true), 4, true));
                }
            }
        }
    }
}

// MAP
void TowerDefense::setup_MAP(std::string const path_itd, int const pixels_in_LINE)
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

void TowerDefense::render_MAP()
{
    this->map.load_MAP();
}

// Active l'interface utilisateurs et les infos
void TowerDefense::active_UI(int &_width, int &_height)
{
    this->ui.show_CELLS(this->map, this->LoadedTextures);
    this->ui.show_ENEMY_PROPERTIES(this->current_WAVE_id, this->current_ENEMIES_in_WAVE);
    this->ui.active_award_if_ENEMY_die(this->current_ENEMIES_in_WAVE);
    this->ui.show_WALLET(_width, _height);
    for (auto &tower : this->TOWERS_ITD)
    {
        this->ui.show_TOWER_to_select(this->map, tower.second);
        this->ui.show_CURSOR_select(this->map, tower.second, this->LoadedTextures);
    }
}

// Récupère la current_WAVE depuis l'ITD avec un id.
void TowerDefense::setup_WAVE()
{
    this->current_WAVE = this->WAVES_ITD.at(this->current_WAVE_id);
    this->WAVES_checked.push_back(this->current_WAVE_id);
    std::cout << "Vague " << this->current_WAVE_id << " => " << this->current_WAVE.number_of_ENDPOINTS << " spawns avec " << this->current_WAVE.number_of_ENEMIES << " ennemis " << std::endl;
}

// Récupère les ennemis de la vague (aléatoirement en fonction des types référencés)
void TowerDefense::get_ENEMIES_into_WAVE()
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
void TowerDefense::setup_ENEMIES_in_WAVE()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (auto &enemy : this->current_ENEMIES_in_WAVE)
        enemy.second.setup(this->map, (std::rand() % this->current_WAVE.number_of_ENDPOINTS), this->LoadedTextures);
    this->TIME_since_last_ENEMY_launched = 0.0f;
}

// Met à jour le comportement des ennemis : Apparition / Blessures / Point de vie
void TowerDefense::update_ENEMIES_in_WAVE(const double &elapsedTime, const double &currentTime)
{
    for (auto &enemy : this->current_ENEMIES_in_WAVE)
    {
        // On active les ennemis un à un avec un délai. (this->current_WAVE.TIME_btw_SPAWN = délai en seconde)
        if (ENEMIES_id_to_launch == enemy.first && currentTime - this->TIME_since_last_ENEMY_launched >= this->current_WAVE.TIME_btw_SPAWN)
        {
            enemy.second.isMoving = true;
            ENEMIES_id_to_launch++;
            this->TIME_since_last_ENEMY_launched = currentTime;
        }

        if (enemy.second.isMoving)
        {
            enemy.second.update(this->map, elapsedTime);
            if (enemy.second.is_burning)
                this->SPRITE_SHEETS_ITD.at("FIRE_ORANGE").updateSpriteSheet(currentTime);
        }
    }
}

// Met à jour et affiche les états de l'ennemi : Position des sprites / Autres sprites liés à l'ennemi.
void TowerDefense::render_ENEMIES_in_WAVE()
{
    for (auto &enemy : this->current_ENEMIES_in_WAVE)
    {
        if (!enemy.second.isDead && enemy.second.isMoving)
        {
            glPushMatrix();
            enemy.second.render(this->map);
            glPopMatrix();

            if (enemy.second.is_burning)
            {
                glPushMatrix();
                glTranslatef(0, 0.01, 0);
                this->SPRITE_SHEETS_ITD.at("FIRE_ORANGE").renderSpriteSheet(enemy.second.pos.x, enemy.second.pos.y, this->map);
                glPopMatrix();
            }
        }
    }
}
// Update des vagues en fonction de l'avancée du jeu
void TowerDefense::update_WAVE()
{
    // Le jeu se termine quand on a effectué toutes les vagues de l'ITD.
    if (this->current_WAVE_id != this->WAVES_ITD.size())
    {
        // Si la WAVE correspondant au current_WAVE_id n'est pas trouvé, alors on la charge !
        if (std::find(this->WAVES_checked.begin(), this->WAVES_checked.end(), this->current_WAVE_id) == this->WAVES_checked.end())
        {
            setup_WAVE();
            get_ENEMIES_into_WAVE();
            setup_ENEMIES_in_WAVE();
        }

        // Si l'ennemi meurt, on le supprime de la vague.
        std::unordered_map<int, Enemy> current_ENEMIES_in_WAVE_copy{this->current_ENEMIES_in_WAVE}; // copy pour pas boucler sur des éléments que l'on delete
        for (auto &enemy : current_ENEMIES_in_WAVE_copy)
            if (enemy.second.isDead)
                this->current_ENEMIES_in_WAVE.erase(enemy.first);

        // Plus d'ennemis dans la vague actuelle ? On passe à la suivante
        if (this->current_ENEMIES_in_WAVE.empty())
        {
            this->ENEMIES_id_to_launch = 0;
            this->current_WAVE_id++;
        }
    }
    else
    {
        std::cout << "FIN DU JEU" << std::endl;
        // écran de fin au lieu d'exit après.
        exit(0);
    }
}
// Met à jour le comportement des tours
void TowerDefense::update_TOWERS(const double &elapsedTime, const double &currentTime)
{
    for (auto &tower : this->current_TOWERS_in_MAP)
        tower.second.update(this);
}

// Met à jour et affiche les états des tours
void TowerDefense::render_TOWERS()
{
    for (auto &tower : this->current_TOWERS_in_MAP)
    {
        glPushMatrix();
        tower.second.render(this->map);
        glPopMatrix();
    }
}

// PARTICLE
void TowerDefense::setup_SPRITE_SHEETS()
{
    for (auto &particle : this->SPRITE_SHEETS_ITD)
        particle.second.loadSpriteSheet(this->LoadedTextures);
}