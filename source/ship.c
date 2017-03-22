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
 */

#define WAS_USING_SHIP
#include "wasteroids.h"


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
                   bool alive, ALLEGRO_COLOR color) {
    Ship * newShip;
    newShip = (Ship *) malloc(sizeof(Ship));

    // Value checking
    newShip->x = x;
    newShip->y = y;
    newShip->direction = direction;
    newShip->speed = speed;

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

    return NULL;
}