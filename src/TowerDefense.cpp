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

// SONS
void TowerDefense::Load_All_Sounds(std::vector<std::pair<std::string, ma_sound *>> &sounds)
{
    ma_result result;
    for (auto &sound : sounds)
    {
        result = ma_sound_init_from_file(&SoundEngine::GetEngine(), sound.first.c_str(), MA_SOUND_FLAG_STREAM, NULL, NULL, sound.second);
        if (result != MA_SUCCESS)
        {
            std::cerr << "Impossible d'initialiser le son : " << sound.first << std::endl;
        }
    }
}

// TEXTURES
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
    this->map.get_PIXELS_from_SCHEMA();
    this->map.set_PIXELS_type();
    this->map.set_PIXELS_connected();
    this->map.check_if_PIXEL_is_NODE();
    // Plus court chemin
    this->map.create_GRAPH_from_NODES();
    this->map.get_SHORTER_PATH_LIST();
    this->map.get_TILES_path_from_PIXELS();
    this->map.get_TILES_MAP(this->LoadedTextures);
}

void TowerDefense::render_MAP()
{
    draw_MAP_background(this->LoadedTextures["images/textures/Map/Background_Map.png"], this->map);
    this->map.draw_MAP();
}

void TowerDefense::setup_BASE()
{
    for (Pixel &pixel : this->map.PIXELS)
        if (pixel.is_START_POINT)
            this->base.pos = {pixel.x, pixel.y};
}

void TowerDefense::render_BASE_health()
{
    draw_BASE_health(this->LoadedTextures["images/textures/Other/Base_Life.png"], this->base, 0.0f, -2.f, this->map);

    // Si la base est détruite => GAME OVER.
    if (this->base.is_Destroyed)
        this->GAME_OVER = true;
}

// Active l'interface utilisateurs et les infos
void TowerDefense::active_UI(int &_width, int &_height)
{
    for (auto &tower : this->TOWERS_ITD)
    {
        this->ui.show_TOWER_to_select(this->map, tower.second, this->LoadedTextures);
        this->ui.show_CURSOR_select(this->map, tower.second, this->LoadedTextures);
    }
    this->ui.show_CURSOR_on_MAP(this->map, this->LoadedTextures);
    // this->ui.show_ENEMY_PROPERTIES(this->current_WAVE_id, this->current_ENEMIES_in_WAVE);
    this->ui.show_ENEMY_PROPERTY(this->map, this->LoadedTextures, this->current_ENEMIES_in_WAVE);
    this->ui.show_TOWER_PROPERTY(this->map, this->LoadedTextures, this->current_TOWERS_in_MAP);
    this->ui.show_WALLET(this->map, this->LoadedTextures);
    this->ui.show_WAVE(this->map, this->current_WAVE_id, this->LoadedTextures);
}

// Récupère la current_WAVE depuis l'ITD avec un id.
void TowerDefense::setup_WAVE()
{
    this->current_WAVE = this->WAVES_ITD.at(this->current_WAVE_id);
    this->WAVES_checked.push_back(this->current_WAVE_id);
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

// Setup des ennemis
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
            enemy.second.update(elapsedTime);
            if (enemy.second.is_burning)
                this->SPRITE_SHEETS_ITD.at("FIRE_ORANGE").updateSpriteSheet(currentTime);
        }

        // Si l'ennemi atteint la Base.
        if (std::round(enemy.second.pos.x) == std::round(this->base.pos.x) && std::round(enemy.second.pos.y) == std::round(this->base.pos.y))
        {
            enemy.second.isDead = true;
            enemy.second.hasReachTarget = true;
        }

        if (enemy.second.isDead && !enemy.second.hasReachTarget) // Gain de WALLET si l'ennemi meurt
            this->ui.WALLET += 10;

        if (enemy.second.isDead && enemy.second.hasReachTarget) // Perte de WALLET si l'ennemi atteint la Base
        {
            this->base.ouch += enemy.second.damage;
            this->ui.WALLET -= 5;
        }
    }

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
            this->FINISHED_WAVE = true;
    }
    else if (!this->GAME_OVER)
    {
        this->PLAYER_WIN = true;
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

// Met à jour le comportement des tours
void TowerDefense::update_TOWERS(const double &elapsedTime, const double &currentTime)
{
    for (auto &tower : this->TOWERS_ITD)
    {
        if (tower.second.type == this->current_TOWER_id)
            tower.second.isSelectedinUI = true;
        else
            tower.second.isSelectedinUI = false;
    }
    // Mise à jour des tours.
    for (auto &tower : this->current_TOWERS_in_MAP)
        tower.second.update(this, elapsedTime, currentTime);

    // Suppression des tours.
    std::unordered_map<int, Tower> current_TOWERS_in_MAP_copy{this->current_TOWERS_in_MAP}; // copy pour pas boucler sur des éléments que l'on delete
    for (auto &tower : current_TOWERS_in_MAP_copy)
        if (tower.second.isDestroyed)
        {
            // Si la tour meurt => On reset les propriétés du pixel où était la tour à VOID.
            for (Pixel &pixel : this->map.PIXELS)
                if (tower.second.pos.x == pixel.x && tower.second.pos.y == pixel.y)
                {
                    pixel.is_VOID = true;
                    pixel.is_TOWER = false;
                }
            // On supprime la tour de la map.
            this->current_TOWERS_in_MAP.erase(tower.first);
        }

    // Si aucune tour n'est posé ET que le joueur ne peut rien acheter => GAME OVER
    if (this->current_TOWERS_in_MAP.empty() && this->ui.WALLET == 0)
        this->GAME_OVER = true;
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