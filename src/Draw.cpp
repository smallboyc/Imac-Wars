#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include "Map.hpp"
#include "Draw.hpp"

// Dessine une texture à la position (x,y) sur une map
void draw_quad_with_texture(GLuint const &texture, float &x, float &y, Map &map)
{
    float centerOffset = map.SEMI_MAP_SIZE - map.PIXEL_SIZE / 2; // Par défaut la case est centrée en (0,0), on doit donc l'ajuster.
    float X0 = (x - map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;
    float X1 = (x + map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;
    float Y0 = (y - map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;
    float Y1 = (y + map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;

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

void draw_enemy(GLuint const &texture, float &x, float &y, Map &map, float health)
{
    // BARRE DE VIE
    float centerOffset = map.SEMI_MAP_SIZE - map.PIXEL_SIZE / 2; // Par défaut la case est centrée en (0,0), on doit donc l'ajuster.
    float X0 = (x - map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;
    float X1 = (x + map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;
    float Y1 = (y + map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;
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

void draw_quad(float &x, float &y, Map &map)
{
    float centerOffset = map.SEMI_MAP_SIZE - map.PIXEL_SIZE / 2;
    float X0 = (x - map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;
    float X1 = (x + map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;
    float Y0 = (y - map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;
    float Y1 = (y + map.SEMI_MAP_SIZE) / map.NUMBER_OF_PIXELS_IN_LINE - centerOffset;

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
    glVertex2d(1, 0);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2d(0, 0);
    glVertex2d(0, 1);
    glEnd();
}