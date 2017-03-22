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


/*=========================================
=            Local definitions            =
=========================================*/

static const float alpha1 = ALLEGRO_PI * 165.0f / 180.0f;
static const float alpha3 = ALLEGRO_PI * 165.0f / 180.0f;
static const float alpha4 = ALLEGRO_PI * 175.0f / 180.0f;

static void print_coords(float x1, float y1, float x2, float y2) {
    printf("\nCoords:\n");
    printf("(x1, y1) = (%.2f, %.2f)\n", x1, y1);
    printf("(x2, y2) = (%.2f, %.2f)\n", x2, y2);
    printf("\n");
}

/*=====  End of Local definitions  ======*/


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
Ship * ship_make_new(float x, float y, float direction, float scale, float speed,
                   bool alive, ALLEGRO_COLOR color, float thickness) {
    Ship * newShip;
    newShip = (Ship *) malloc(sizeof(Ship));

    // Value checking
    newShip->x = x;
    newShip->y = y;
    newShip->direction = direction;
    newShip->scale = scale;
    newShip->speed = speed;
    newShip->alive = alive;
    newShip->color = color;
    newShip->thickness = thickness;

    return newShip;
}

/**
 * @brief      Creates a new Ship with default values
 *
 * @return     Pointer to new Ship
 */
Ship * ship_make_new_default() {
    Ship * newShip;
    float x = 100.0f;
    float y = 100.0f;
    float direction = 0.0f;
    float scale = 2.0f;
    float speed = 3.0f;
    bool alive = true;
    ALLEGRO_COLOR color = SHIP_COLOR;
    float thickness = 3.0f;

    newShip = ship_make_new(x, y, direction, scale, speed, alive,
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
    /* 7 base points for ship drawing
     *
     *    POINTS LOCATION
     *    C stands for Center Point
     *    
     *          [0]
     *    
     *    
     *           C
     *     [3][4] [5][6]
     *     
     *    [1]         [2]
     *
     */
    float x[7];
    float y[7];

    // Points are relative to center and ship's direction
    float x_center;
    float y_center;
    float dir;
    float scale;

    int32 width;
    int32 height;

    // Shouldn't draw if ship wasn't alive
    if (!ship->alive) {
        return -1;
    }

    x_center = ship->x;
    y_center = ship->y;
    dir = ship->direction;
    scale = ship->scale;

    // If it crosses the border, make it apper on the other side
    width = al_get_display_width(screen);
    height = al_get_display_height(screen);
    if (x_center > width) {
        x_center = 0;
        ship->x = 0;
    }
    else if (x_center < 0) {
        x_center = width;
        ship->x = width;
    }

    if (y_center > height) {
        y_center = 0;
        ship->y = 0;
    }
    else if (y_center < 0) {
        y_center = height;
        ship->y = height;
    }

    // Get base points
    x[0] = x_center;
    y[0] = y_center;

    x[1] = x_center + scale * SHIP_DIMENSION * (float) cos(dir + alpha1);
    y[1] = y_center - scale * SHIP_DIMENSION * (float) sin(dir + alpha1);

    x[3] = x_center + scale * SHIP_DIMENSION * (float) cos(dir + alpha3) * 2.0f / 3.0f;
    y[3] = y_center - scale * SHIP_DIMENSION * (float) sin(dir + alpha3) * 2.0f / 3.0f;

    x[4] = x_center + scale * SHIP_DIMENSION * (float) cos(dir + alpha4) * 2.0f / 3.0f;
    y[4] = y_center - scale * SHIP_DIMENSION * (float) sin(dir + alpha4) * 2.0f / 3.0f;

    x[2] = x_center + scale * SHIP_DIMENSION * (float) cos(dir - alpha1);
    y[2] = y_center - scale * SHIP_DIMENSION * (float) sin(dir - alpha1);

    x[6] = x_center + scale * SHIP_DIMENSION * (float) cos(dir - alpha3) * 2.0f / 3.0f;
    y[6] = y_center - scale * SHIP_DIMENSION * (float) sin(dir - alpha3) * 2.0f / 3.0f;

    x[5] = x_center + scale * SHIP_DIMENSION * (float) cos(dir - alpha4) * 2.0f / 3.0f;
    y[5] = y_center - scale * SHIP_DIMENSION * (float) sin(dir - alpha4) * 2.0f / 3.0f;

    // Draws the four line segments
    al_draw_line(x[0], y[0], x[1], y[1], ship->color, ship->thickness);
    al_draw_line(x[0], y[0], x[2], y[2], ship->color, ship->thickness);
    al_draw_line(x[3], y[3], x[4], y[4], ship->color, ship->thickness);
    al_draw_line(x[5], y[5], x[6], y[6], ship->color, ship->thickness);

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

/**
 * @brief      Moves the ship
 *
 * @param      ship  The ship
 */
void ship_move(Ship *ship) {
    float dx;
    float dy;

    dx = 0.0f;
    dy = 0.0f;

    if (pressed_keys[ALLEGRO_KEY_UP]) {
        dx = ship->speed * cos(ship->direction);
        dy = - ship->speed * sin(ship->direction);
    }
    
    if (pressed_keys[ALLEGRO_KEY_LEFT]) {
        ship->direction += DIRECTION_STEP;
        if (ship->direction >= MAX_ANGLE) {
            ship->direction = 0.0f + (ship->direction - MAX_ANGLE);
        }
    }

    if (pressed_keys[ALLEGRO_KEY_RIGHT]) {
        ship->direction -= DIRECTION_STEP;
        if (ship->direction < 0.0f) {
            ship->direction = MAX_ANGLE + ship->direction;
        }
    }

    ship->x += dx;
    ship->y += dy;
}
