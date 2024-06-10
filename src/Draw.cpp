#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include "Map.hpp"
#include "Draw.hpp"

void draw_MAP_background(GLuint const &texture, Map &map)
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(-map.SEMI_MAP_SIZE, -map.SEMI_MAP_SIZE);

    glTexCoord2d(1, 0);
    glVertex2f(map.SEMI_MAP_SIZE, -map.SEMI_MAP_SIZE);

    glTexCoord2d(1, 1);
    glVertex2f(map.SEMI_MAP_SIZE, map.SEMI_MAP_SIZE);

    glTexCoord2d(0, 1);
    glVertex2f(-map.SEMI_MAP_SIZE, map.SEMI_MAP_SIZE);

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

// Dessine une texture à la position (x,y) sur une map
void draw_quad_with_texture(GLuint const &texture, float &x, float &y, Map &map)
{
    float X0 = x / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float X1 = X0 + map.PIXEL_SIZE;
    float Y0 = y / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float Y1 = Y0 + map.PIXEL_SIZE;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(X0, Y0);

    glTexCoord2d(1, 0);
    glVertex2f(X1, Y0);

    glTexCoord2d(1, 1);
    glVertex2f(X1, Y1);

    glTexCoord2d(0, 1);
    glVertex2f(X0, Y1);

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void draw_BASE_health(Base &base, float const &x, float const &y, Map &map)
{
    // BARRE DE FOND NOIRE
    float X0 = x / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float X1 = X0 + map.MAP_SIZE;
    float Y0 = y / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float Y1 = Y0 + map.PIXEL_SIZE;

    glEnable(GL_TEXTURE_2D);
    glColor4ub(0, 0, 0, 255);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(X0, Y0);

    glTexCoord2d(1, 0);
    glVertex2f(X1, Y0);

    glTexCoord2d(1, 1);
    glVertex2f(X1, Y1);

    glTexCoord2d(0, 1);
    glVertex2f(X0, Y1);
    glEnd();
    glPopMatrix();

    // BARRE DE VIE
    X0 = x / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    X1 = X0 + map.MAP_SIZE - base.ouch / base.health;
    Y0 = y / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    Y1 = Y0 + map.PIXEL_SIZE;

    // Si la barre de vie est vide = GAME OVER
    if (X1 <= X0)
        base.is_Destroyed = true;

    if (!base.is_Destroyed)
    {
        glEnable(GL_TEXTURE_2D);
        glColor4ub(0 + 200 * base.ouch / base.health, 200 - 200 * base.ouch / base.health, 0, 255);
        glPushMatrix();
        glBegin(GL_QUADS);
        glTexCoord2d(0, 0);
        glVertex2f(X0, Y0);

        glTexCoord2d(1, 0);
        glVertex2f(X1, Y0);

        glTexCoord2d(1, 1);
        glVertex2f(X1, Y1);

        glTexCoord2d(0, 1);
        glVertex2f(X0, Y1);
        glEnd();
        glPopMatrix();
    }
}

void draw_enemy(GLuint const &texture, Enemy &enemy, float &x, float &y, Map &map, float &health, float &hit)
{
    // BARRE DE VIE
    float X0 = x / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float X1 = X0 + map.PIXEL_SIZE - hit / health;
    float Y0 = y / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float Y1 = Y0 + map.PIXEL_SIZE;

    // Si la barre de vie est vide = l'ennemi meurt
    if (X1 <= X0)
        enemy.isDead = true;

    // Si l'ennemi n'est pas mort on le dessine.
    if (!enemy.isDead)
    {
        glColor3ub(255, 0, 0); // Couleur rouge pour la barre de vie
        glPushMatrix();
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(X0, Y1); // Coin supérieur gauche
        glVertex2f(X1, Y1); // Coin supérieur droit
        glEnd();
        glPopMatrix();
        draw_quad_with_texture(texture, x, y, map);
    }
}

void draw_tower(GLuint const &texture, Tower &tower, float &x, float &y, Map &map, float &health, float &age)
{
    // BARRE DE VIE
    float X0 = x / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float X1 = X0 + map.PIXEL_SIZE - age / health;
    float Y0 = y / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float Y1 = Y0 + map.PIXEL_SIZE;

    // Si la barre de vie est vide = la tour est détruite.
    if (X1 <= X0)
        tower.isDestroyed = true;

    // Si la tour n'est pas détruite on la dessine.
    if (!tower.isDestroyed)
    {
        glColor3ub(0, 255, 0); // Couleur verte pour la barre de vie
        glPushMatrix();
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(X0, Y1); // Coin supérieur gauche
        glVertex2f(X1, Y1); // Coin supérieur droit
        glEnd();
        glPopMatrix();
        draw_quad_with_texture(texture, x, y, map);
    }
}

void draw_spriteSheet(SpriteSheet const &spriteSheet, float const &x, float const &y, Map &map)
{
    float X0 = x / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float X1 = X0 + map.PIXEL_SIZE;
    float Y0 = y / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float Y1 = Y0 + map.PIXEL_SIZE;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spriteSheet.texture);
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2d(spriteSheet.next_frame_in_LINE, spriteSheet.next_frame_in_COL);
    glVertex2f(X0, Y0);

    glTexCoord2d(spriteSheet.next_frame_in_LINE + spriteSheet.frame_width, spriteSheet.next_frame_in_COL);
    glVertex2f(X1, Y0);

    glTexCoord2d(spriteSheet.next_frame_in_LINE + spriteSheet.frame_width, spriteSheet.next_frame_in_COL + spriteSheet.frame_height);
    glVertex2f(X1, Y1);

    glTexCoord2d(spriteSheet.next_frame_in_LINE, spriteSheet.next_frame_in_COL + spriteSheet.frame_height);
    glVertex2f(X0, Y1);

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void draw_quad(float &x, float &y, Map &map)
{
    float X0 = x / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float X1 = X0 + map.PIXEL_SIZE;
    float Y0 = y / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float Y1 = Y0 + map.PIXEL_SIZE;

    glEnable(GL_TEXTURE_2D);

    for (Pixel &pixel : map.PIXELS)
    {
        // Si on trouve un pixel à la position de notre cible et que la zone est constructible, on set le G et on break
        if (pixel.x == x && pixel.y == y && pixel.is_VOID)
        {
            glColor4ub(0, 255, 0, 128);
            break;
        } // Si la zone n'est pas constructible, on set le R
        else
            glColor4ub(255, 0, 0, 128);
    }
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(X0, Y0);

    glTexCoord2d(1, 0);
    glVertex2f(X1, Y0);

    glTexCoord2d(1, 1);
    glVertex2f(X1, Y1);

    glTexCoord2d(0, 1);
    glVertex2f(X0, Y1);

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void draw_UI_ITEM(GLuint const &texture, float const &x, float const &y, int const &item_size, Map &map)
{
    float X0 = x / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float X1 = X0 + item_size * map.PIXEL_SIZE;
    float Y0 = y / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE;
    float Y1 = Y0 + item_size * map.PIXEL_SIZE;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(X0, Y0);

    glTexCoord2d(1, 0);
    glVertex2f(X1, Y0);

    glTexCoord2d(1, 1);
    glVertex2f(X1, Y1);

    glTexCoord2d(0, 1);
    glVertex2f(X0, Y1);

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}


void draw_BREAK_MENU(Map &map)
{
    glEnable(GL_TEXTURE_2D);

    glColor4ub(0, 0, 0, 128);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(-map.SEMI_MAP_SIZE, -map.SEMI_MAP_SIZE);

    glTexCoord2d(1, 0);
    glVertex2f(map.SEMI_MAP_SIZE, -map.SEMI_MAP_SIZE);

    glTexCoord2d(1, 1);
    glVertex2f(map.SEMI_MAP_SIZE, map.SEMI_MAP_SIZE);

    glTexCoord2d(0, 1);
    glVertex2f(-map.SEMI_MAP_SIZE, map.SEMI_MAP_SIZE);

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void drawOrigin()
{
    glPointSize(10);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2d(0, 0);
    glVertex2d(0.5, 0);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2d(0, 0);
    glVertex2d(0, 0.5);
    glEnd();
}