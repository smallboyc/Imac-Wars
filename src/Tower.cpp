#include <Tower.hpp>
#include <Draw.hpp>
#include "TowerDefense.hpp"

void Tower::setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    bullet.setup(LoadedTextures);
    bullet.pos = pos;
}

void Tower::update(const double &elapsedTime, const double &currentTime, TowerDefense* TD)
{
    fireRate -= 0.1;

    for(auto& enemy : TD->current_ENEMIES_in_WAVE)
    {
        // Distance de Chebyshev
        if(std::max(std::abs(pos.x - enemy.second.pos.x), std::abs(pos.y - enemy.second.pos.y)) < 2)
        {
            bullet.update(enemy.second);
            bullet.isBeingShot = true;
            break;
        }        
        bullet.isBeingShot = false;
    }

    if(fireRate < 0)
    {
        bullet.pos = pos;
        fireRate = 3;
        bullet.fixedDirection = false;
    }
}

void Tower::draw(Map& map)
{
    draw_tower(this->texture, this->pos.x, this->pos.y, map);

    if(bullet.isBeingShot)
        bullet.draw(map);
}