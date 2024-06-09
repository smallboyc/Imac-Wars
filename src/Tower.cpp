#include <Tower.hpp>
#include <Draw.hpp>
#include "TowerDefense.hpp"

void Tower::setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures, glm::vec2 const &pixel_pos)
{
    this->pos = pixel_pos;
    this->bullet.setup(LoadedTextures, this->pos);
}

void Tower::update(TowerDefense *TD, const double &elapsedTime)
{
    fireRate -= elapsedTime * 3;

    for (auto &enemy : TD->current_ENEMIES_in_WAVE)
    {
        // Distance de Chebyshev
        if (std::max(std::abs(pos.x - enemy.second.pos.x), std::abs(pos.y - enemy.second.pos.y)) < 2 && enemy.second.isMoving)
        {
            this->bullet.update(enemy.second, elapsedTime);
            this->bullet.isBeingShot = true;
            break;
        }
        this->bullet.isBeingShot = false;
    }

    // Réinitialise l'état du laser si on touche l'ennemi OU le laser termine son trajet.
    if (fireRate < 0)
    {
        this->bullet.pos = this->pos;
        fireRate = 3;
        this->bullet.fixedDirection = false;
        this->bullet.hitEnemy = false;
    }
}

void Tower::render(Map &map)
{
    draw_tower(this->texture, this->pos.x, this->pos.y, map);

    if (this->bullet.isBeingShot)
        this->bullet.render(map);
}