#include <Tower.hpp>
#include <Draw.hpp>

void Tower::setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    pos.x = 7;
    pos.y = 7;
    std::string texturePath = "images/textures/Tower/tower.png";
    texture = LoadedTextures[texturePath];
}

void Tower::update()
{
    
}

void Tower::draw(Map& map)
{
    draw_tower(this->texture, this->pos.x, this->pos.y, map);
}