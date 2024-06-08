#include <Tower.hpp>
#include <Draw.hpp>
#include "TowerDefense.hpp"

void Tower::setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    std::string texturePath = "images/textures/Tower/tower.png";
    texture = LoadedTextures[texturePath];
}

void Tower::update(const double &elapsedTime, const double &currentTime, TowerDefense* TD)
{
    for(auto& enemy : TD->current_ENEMIES_in_WAVE)
    {
        if(glm::distance(pos, enemy.second.pos) < 1)
        {
            std::cout << "Enemy nearby!\n";
        }
    }
}

void Tower::draw(Map& map)
{
    draw_tower(this->texture, this->pos.x, this->pos.y, map);
}