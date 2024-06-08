#include "Bullet.hpp"
#include "TowerDefense.hpp"

void Bullet::setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    std::string texturePath = "images/textures/Tower/bullet.png";
    texture = LoadedTextures[texturePath];
}

void Bullet::update(Enemy& enemy)
{
    if(!fixedDirection)
    {
        direction = {enemy.pos.x - pos.x, enemy.pos.y - pos.y};
        fixedDirection = true;
    }

    pos.x += direction.x/10;
    pos.y += direction.y/10;
}

void Bullet::draw(Map& map)
{
    draw_quad_with_texture(texture, pos.x, pos.y, map);
}