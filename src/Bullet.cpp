#include "Bullet.hpp"
#include "TowerDefense.hpp"

void Bullet::setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures, glm::vec2 &tower_pos, int tower_id)
{
    this->pos = tower_pos;

    switch(tower_id)
    {
        case 0:
            this->texture = LoadedTextures["images/textures/Tower/bullet.png"];
            break;
        case 1:
            this->texture = LoadedTextures["images/textures/Tower/stoneBall.png"];
            break;
        case 2:
            this->texture = LoadedTextures["images/textures/Tower/cannonBall.png"];
            break;
        default:
            this->texture = LoadedTextures["images/textures/Tower/bullet.png"];
    }
}

void Bullet::update(Enemy &enemy, const double &elapsedTime, float degats)
{
    // Si le laser touche l'ennemi
    if (std::round(enemy.pos.x) == std::round(this->pos.x) && std::round(enemy.pos.y) == std::round(this->pos.y) && !(this->hitEnemy))
    {
        // std::cout << "HIT" << std::endl;
        enemy.hit += degats;
        this->hitEnemy = true;

        pos = {1000, 1000};
    }
    if (enemy.health/enemy.hit <= enemy.health/3)
    {
        enemy.is_burning = true;
    }
    

    if (!fixedDirection)
    {
        direction = {enemy.pos.x - pos.x, enemy.pos.y - pos.y};
        fixedDirection = true;
    }

    pos.x += direction.x * elapsedTime * 7;
    pos.y += direction.y * elapsedTime * 7;
}

void Bullet::render(Map &map)
{
    draw_quad_with_texture(texture, pos.x, pos.y, map);
}