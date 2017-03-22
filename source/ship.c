/*
 *
 * MIT License
 * 
 * Copyright (c) 2017 Wilk Maia
 * wilkmaia [at] gmail [dot] com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

/**
 * Spaceship functions
 * 
 * TODO
 * Make a "drawable" class for drawable objects (ship, blasts, asteroids)
 * to inherit.
 */

#define WAS_USING_SHIP
#include "wasteroids.h"


static void print_coords(float x1, float y1, float x2, float y2) {
    printf("\nCoords:\n");
    printf("(x1, y1) = (%.2f, %.2f)\n", x1, y1);
    printf("(x2, y2) = (%.2f, %.2f)\n", x2, y2);
    printf("\n");
}


/**
 * @brief      Creates a new Ship
 *
 * @param[in]  x          x position
 * @param[in]  y          y position
 * @param[in]  direction  The direction
 * @param[in]  speed      The speed
 * @param[in]  alive      The alive
 * @param[in]  color      The color
 *
 * @return     Pointer to new Ship
 */
Ship * ship_make_new(float x, float y, float direction, float speed,
                   bool alive, ALLEGRO_COLOR color, float thickness) {
    Ship * newShip;
    newShip = (Ship *) malloc(sizeof(Ship));

    // Value checking
    newShip->x = x;
    newShip->y = y;
    newShip->direction = direction;
    newShip->speed = speed;
    newShip->alive = alive;
    newShip->color = color;
    newShip->thickness = thickness;

    return newShip;
}

Ship * ship_make_new_default() {
    Ship * newShip;
    float x = 100.0f;
    float y = 100.0f;
    float direction = 0.0f;
    float speed = 3.0f;
    bool alive = true;
    ALLEGRO_COLOR color = al_map_rgb(0, 255, 0);
    float thickness = 3.0f;

    newShip = ship_make_new(x, y, direction, speed, alive,
                            color, thickness);

    return newShip;
}

/**
 * @brief      Draws ship on screen
 *
 * @param      ship  The ship
 *
 * @return     0 for success or anything else for error
 */
int8 ship_draw(Ship *ship) {
    float x1;
    float y1;
    float x2;
    float y2;

    // Draws first line
    x1 = ship->x - 8.0f;
    y1 = ship->y + 9.0f;
    x2 = ship->x;
    y2 = ship->y - 11.0f;
    al_draw_line(x1, y1, x2, y2, ship->color, ship->thickness);

    // Draws second line
    x1 = ship->x + 8.0f;
    y1 = ship->y + 9.0f;
    x2 = ship->x;
    y2 = ship->y - 11.0f;
    al_draw_line(x1, y1, x2, y2, ship->color, ship->thickness);

    // Draws third line
    x1 = ship->x - 6.0f;
    y1 = ship->y + 4.0f;
    x2 = ship->x - 1.0f;
    y2 = ship->y + 4.0f;
    al_draw_line(x1, y1, x2, y2, ship->color, ship->thickness);

    // Draws fourth line
    x1 = ship->x + 6.0f;
    y1 = ship->y + 4.0f;
    x2 = ship->x + 1.0f;
    y2 = ship->y + 4.0f;
    al_draw_line(x1, y1, x2, y2, ship->color, ship->thickness);

    return 0;
}

/**
 * @brief      Deletes ship, freeing its memory
 *
 * @param      ship  The ship
 *
 * @return     Pointer to object. Should point to NULL if everything went ok.
 */
Ship * ship_delete(Ship *ship) {
    free(ship);
    ship = NULL;

    return ship;
}

void ship_move(Ship *ship) {
    float dx;
    float dy;

    dx = 0.0f;
    dy = 0.0f;

    if (pressed_keys[ALLEGRO_KEY_UP]) {
        dx = ship->speed * cos(ship->direction);
        dy = ship->speed * sin(ship->direction);
    }
    
    if (pressed_keys[ALLEGRO_KEY_RIGHT]) {
        ship->direction += DIRECTION_STEP;
        if (ship->direction >= 2 * ALLEGRO_PI) {
            ship->direction = 0.0f + (ship->direction - 2 * ALLEGRO_PI);
        }
    }

    if (pressed_keys[ALLEGRO_KEY_LEFT]) {
        ship->direction -= DIRECTION_STEP;
        if (ship->direction < 0.0f) {
            ship->direction = 2 * ALLEGRO_PI  + ship->direction;
        }
    }

    ship->x += dx;
    ship->y += dy;
}
