#include "Bullet.hpp"

void Bullet::setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    std::string texturePath = "images/textures/Tower/bullet.png";
    texture = LoadedTextures[texturePath];
}

void Bullet::update()
{
    
}

void Bullet::draw(Map& map)
{

}