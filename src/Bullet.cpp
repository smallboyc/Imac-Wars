#include "Bullet.hpp"
#include "TowerDefense.hpp"

void Bullet::setup(std::unordered_map<std::string, SpriteSheet> &SPRITE_SHEETS_ITD, Tower* tower)
{
    this->pos = tower->pos;

    switch (tower->type)
    {
    case 0:
        this->sprite = SPRITE_SHEETS_ITD.at("BULLET_BLUE");
        break;
    case 1:
        this->sprite = SPRITE_SHEETS_ITD.at("BULLET_BLUE_2");
        break;
    case 2:
        this->sprite = SPRITE_SHEETS_ITD.at("FIRE_BLUE");
        break;
    default:
        this->sprite = SPRITE_SHEETS_ITD.at("BULLET_BLUE");
    }
}

void Bullet::update(Enemy& enemy, const double &elapsedTime, const double &currentTime, Tower* tower)
{
    // Si le laser touche l'ennemi
    if (std::round(enemy.pos.x) == std::round(this->pos.x) && std::round(enemy.pos.y) == std::round(this->pos.y) && !(this->hitEnemy))
    {
        // std::cout << "HIT" << std::endl;
        if(tower->type != 1)
        {
            enemy.hit += tower->degats;
        }
        else
        {
            enemy.speed /= 3;
            enemy.isTarget = false;
        }

        this->hitEnemy = true;
        pos = {1000, 1000};
    }
    if (enemy.health / enemy.hit <= enemy.health / 3)
    {
        enemy.is_burning = true;
    }

    this->sprite.updateSpriteSheet(currentTime);

    if (!fixedDirection)
    {
        direction = {enemy.pos.x - pos.x, enemy.pos.y - pos.y};
        fixedDirection = true;
    }

    this->pos.x += direction.x * elapsedTime * 4;
    this->pos.y += direction.y * elapsedTime * 4;
}

void Bullet::render(Map &map)
{
    // On dessine la bullet si elle est dans la map.
    if (is_inside_MAP(this->pos.x, this->pos.y, map))
        this->sprite.renderSpriteSheet(this->pos.x, this->pos.y, map);
}