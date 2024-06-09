#include "Bullet.hpp"
#include "TowerDefense.hpp"

void Bullet::setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures, glm::vec2 &tower_pos)
{
    this->pos = tower_pos;
    this->texture = LoadedTextures["images/textures/Tower/bullet.png"];
}

void Bullet::update(Enemy &enemy, const double &elapsedTime)
{
    // Si le laser touche l'ennemi
    if (std::round(enemy.pos.x) == std::round(this->pos.x) && std::round(enemy.pos.y) == std::round(this->pos.y) && !(this->hitEnemy))
    {
        // std::cout << "HIT" << std::endl;
        enemy.hit += 1;
        this->hitEnemy = true;
    }
    if (!fixedDirection)
    {
        direction = {enemy.pos.x - pos.x, enemy.pos.y - pos.y};
        fixedDirection = true;
    }

    pos.x += direction.x * elapsedTime * 10;
    pos.y += direction.y * elapsedTime * 10;
}

void Bullet::render(Map &map)
{
    draw_quad_with_texture(texture, pos.x, pos.y, map);
}