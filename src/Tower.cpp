#include <Tower.hpp>
#include <Draw.hpp>
#include "TowerDefense.hpp"

void Tower::setup(std::unordered_map<std::string, SpriteSheet> &SPRITE_SHEETS_ITD, glm::vec2 const &pixel_pos)
{
    this->pos = pixel_pos;
    this->bullet.setup(SPRITE_SHEETS_ITD, this);
}

void Tower::update(TowerDefense *TD, const double &elapsedTime, const double &currentTime)
{     
    if (currentTime - previousTimeTower >= 2) // On décrémente la vie de la tour toutes les 2 secondes.
    {
        this->age += 1;
        previousTimeTower = currentTime;
    }

    cadence -= elapsedTime * 3;

    if(!lockedEnemy)
    {
        lockedEnemy = true;

        for (auto &enemy : TD->current_ENEMIES_in_WAVE)
        {
            if (enemy.second.isTarget)
            {
                // Distance de Chebyshev
                float dist_to_enemy = std::max(std::abs(pos.x - enemy.second.pos.x), std::abs(pos.y - enemy.second.pos.y));

                if (dist_to_enemy < this->portee)
                {
                    if(dist_to_enemy < closest_enemy_dist)
                    {
                        closest_enemy_dist = dist_to_enemy;
                        closest_enemy = &enemy.second;
                    }
                }
            }
            else
            {
                if (!enemy.second.reSpeedSet)
                {
                    enemy.second.reSpeed = currentTime;
                    enemy.second.reSpeedSet = true;
                }

                if (currentTime - enemy.second.reSpeed >= 2)
                {
                    enemy.second.speed *= 3;

                    enemy.second.isTarget = true;
                    enemy.second.reSpeedSet = false;
                }
            }
        }
    }
    
    if(closest_enemy->isMoving && closest_enemy_dist < this->portee && closest_enemy != nullptr)
    {
        this->bullet.update(*closest_enemy, elapsedTime, currentTime, this);
        this->bullet.isBeingShot = true;
    }
    else
        this->bullet.isBeingShot = false;

    // Réinitialise l'état du laser si le laser termine son trajet.
    if (cadence < 0)
    {
        this->bullet.pos = this->pos;
        cadence = 3;
        this->bullet.fixedDirection = false;
        this->bullet.hitEnemy = false;
        this->bullet.isBeingShot = false;
        lockedEnemy = false;
        closest_enemy_dist = 100;
    }
}

void Tower::render(Map &map)
{
    draw_tower(this->texture, *this, this->pos.x, this->pos.y, map, this->health, this->age);

    if (this->bullet.isBeingShot)
        this->bullet.render(map);
}