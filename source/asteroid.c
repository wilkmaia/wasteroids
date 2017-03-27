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
 * Asteroid functions
 * 
 * TODO
 */

#define WAS_USING_ASTEROID
#define WAS_USING_BLAST
#define WAS_USING_SHIP
#include "wasteroids.h"


/**
 * @brief      Creates a new asteroid
 *
 * @param[in]  x          x position
 * @param[in]  y          y position
 * @param[in]  direction  The direction
 * @param[in]  speed      The speed
 * @param[in]  alive      The alive
 * @param[in]  color      The color
 *
 * @return     Pointer to new asteroid
 */
Asteroid * asteroid_make_new(float x, float y, float direction, float scale, float speed,
                       bool alive, ALLEGRO_COLOR color, float thickness) {
    Asteroid * newAsteroid;
    newAsteroid = (Asteroid *) malloc(sizeof(Asteroid));

    // Value checking
    newAsteroid->x = x;
    newAsteroid->y = y;
    newAsteroid->direction = direction;
    newAsteroid->scale = scale;
    newAsteroid->speed = speed;
    newAsteroid->alive = alive;
    newAsteroid->color = color;
    newAsteroid->thickness = thickness;

    // Add new asteroid to asteroid list
    asteroids[num_asteroids] = newAsteroid;
    ++num_asteroids;

    return newAsteroid;
}

/**
 * @brief      Creates a new asteroid with default setup
 *
 * @param[in]  x          center x-coordinate
 * @param[in]  y          center y-coordinate
 * @param[in]  direction  The direction
 * @param[in]  scale      The scale
 *
 * @return     Pointer to new asteroid
 */
Asteroid * asteroid_make_new_default(float x, float y, float direction, float scale) {
    Asteroid * newAsteroid;
    float speed = 5.0f - scale;
    bool alive = true;
    ALLEGRO_COLOR color = ASTEROID_COLOR;
    float thickness = 3.0f;

    newAsteroid = asteroid_make_new(x, y, direction, scale, speed, alive,
                                    color, thickness);

    return newAsteroid;
}

/**
 * @brief      Draws asteroid on screen
 *
 * @param      asteroid  The asteroid
 *
 * @return     0 for success or anything else for error
 */
int8 asteroid_draw(Asteroid *asteroid) {
    int32 i;

    ALLEGRO_TRANSFORM transform;
    const ALLEGRO_TRANSFORM * prevTransform;

    // Shouldn't draw if asteroid wasn't alive
    if (!asteroid->alive) {
        return -1;
    }

    // Saves current transform state
    prevTransform = al_get_current_transform();

    // Transforms based on asteroid info
    al_identity_transform(&transform);
    al_scale_transform(&transform, asteroid->scale, asteroid->scale);
    al_rotate_transform(&transform, -asteroid->direction + ALLEGRO_PI / 2.0f);
    al_translate_transform(&transform, asteroid->x, asteroid->y);
    al_use_transform(&transform);

    // Draws the asteroid
    for (i = 0; i < NUM_VERTICES - 1; ++i) {
        al_draw_line(VERTICES[2*i], VERTICES[2*i + 1], VERTICES[2*(i+1)], VERTICES[2*(i+1) + 1], asteroid->color, asteroid->thickness);
    }
    al_draw_line(VERTICES[0], VERTICES[1], VERTICES[2*i], VERTICES[2*i + 1], asteroid->color, asteroid->thickness);

    // Reloads previous transform state
    if (prevTransform != NULL) {
        al_use_transform(prevTransform);
    }

    return 0;
}

/**
 * @brief      Draws all active asteroids to screen
 */
void asteroid_draw_all() {
    int32 i;

    for (i = 0; i < num_asteroids; ++i) {
        asteroid_draw(asteroids[i]);
    }
}

/**
 * @brief      Sets the corners of the asteroid on the addresses passed as arguments
 *
 * @param      asteroid  The asteroid
 * @param[out] x1        Address to top left corner's x-coordinate
 * @param[out] y1        Address to top left corner's y-coordinate
 * @param[out] x2        Address to bottom right corner's x-coordinate
 * @param[out] y2        Address to bottom right corner's y-coordinate
 */
void asteroid_get_corners(Asteroid *asteroid, float *x1, float *y1, float *x2, float *y2) {
    *x1 = asteroid->x - asteroid->scale * ASTEROID_DIMENSION;
    *y1 = asteroid->y - asteroid->scale * ASTEROID_DIMENSION;
    *x2 = asteroid->x + asteroid->scale * ASTEROID_DIMENSION;
    *y2 = asteroid->y + asteroid->scale * ASTEROID_DIMENSION;
}

/**
 * @brief      Deletes asteroid, freeing its memory
 *
 * @param      asteroid  The asteroid
 *
 * @return     Pointer to object. Should point to NULL if everything went ok.
 */
Asteroid * asteroid_delete(Asteroid *asteroid) {
    int32 i;

    // TODO
    // Implement a linked list instead maybe
    for (i = 0; i < num_asteroids; ++i) {
        if (asteroids[i] == asteroid) {
            break; // Found the element
        }
    }
    if (i < num_asteroids) { // Element found
        int32 j;
        for (j = i + 1; j < num_asteroids; ++j) {
            asteroids[j-1] = asteroids[j];
        }
        --num_asteroids;
    }

    free(asteroid);
    asteroid = NULL;

    return asteroid;
}

/**
 * @brief      Delete all asteroids on list
 */
void asteroid_delete_all() {
    int32 i;

    for (i = 0; i < num_asteroids; ++i) {
        free(asteroids[i]);
        asteroids[i] = NULL;
    }

    num_asteroids = 0;
}

/**
 * @brief      Move asteroid
 *
 * @param      asteroid  The asteroid
 */
void asteroid_move(Asteroid *asteroid) {
    float x_center = asteroid->x;
    float y_center = asteroid->y;
    float dx;
    float dy;

    int32 width;
    int32 height;

    // No need to waste time here if asteroid isn't alive
    if (!asteroid->alive) {
        asteroid_delete(asteroid);
        return;
    }

    // If it crosses the border, make it appears on the other side
    width = al_get_display_width(screen);
    height = al_get_display_height(screen);
    if (x_center > width) {
        x_center = 0;
        asteroid->x = 0;
    }
    else if (x_center < 0) {
        x_center = width;
        asteroid->x = width;
    }

    if (y_center > height) {
        y_center = 0;
        asteroid->y = 0;
    }
    else if (y_center < 0) {
        y_center = height;
        asteroid->y = height;
    }

    dx = asteroid->speed * cos(asteroid->direction);
    dy = - asteroid->speed * sin(asteroid->direction);

    asteroid->x += dx;
    asteroid->y += dy;
}

/**
 * @brief      Move all asteroids
 */
void asteroid_move_all() {
    int32 i;

    for (i = 0; i < num_asteroids; ++i) {
        asteroid_move(asteroids[i]);
    }
}

/**
 * @brief      Populates the asteroid list with _n_ asteroids
 *
 * @param[in]  n     Number of asteroids
 */
void asteroid_populate(int32 n) {
    int32 i;

    float x;
    float y;
    float direction;
    float scale;

    for (i = 0; i < n; ++i) {
        // Randomly populates
        x = rand() % al_get_display_width(screen);
        y = rand() % al_get_display_height(screen);
        direction = MAX_ANGLE * ((rand() % 100) / 100.0f);
        scale = 1.0f + ((rand() % 11) / 5.0f);

        // Make new asteroid
        asteroid_make_new_default(x, y, direction, scale);
    }
}

/**
 * @brief      Checks if the asteroid and blast collided
 *
 * @param      asteroid  The asteroid
 * @param      blast     The blast
 *
 * @return     true if collision detected; false otherwise
 */
bool asteroid_check_collision_on_blast(Asteroid *asteroid, Blast *blast) {
    // Blast's end point
    float x_blast;
    float y_blast;

    // Asteroid's corners
    float x1;
    float y1;
    float x2;
    float y2;

    // Gets coordinates
    blast_get_end_point(blast, &x_blast, &y_blast);
    asteroid_get_corners(asteroid, &x1, &y1, &x2, &y2);

    // Checks for collision on both points of the blast (start and end points)
    if (common_check_collision(x_blast, y_blast, x1, y1, x2, y2)
            || common_check_collision(blast->x, blast->y, x1, y1, x2, y2)) {
        return true;
    }

    return false;
}

/**
 * @brief      Handles collision on asteroid
 *
 * @param      asteroid  The asteroid
 */
void asteroid_was_hit(Asteroid *asteroid) {
    float direction;
    float scale;
    float x;
    float y;

    // It's time to say goodbye...
    if (asteroid->scale <= 1) {
        asteroid_delete(asteroid);
        return;
    }

    // Otherwise...
    // It gives birth to two smaller children before going away... forever
    // Child 1
    direction = asteroid->direction + ((rand()%101)-50.0f)/100.0f; // Some randomness inserted
    scale = asteroid->scale / 2.0f;
    x = asteroid->x + (rand()%100) - 50.0f;
    y = asteroid->y + (rand()%100) - 50.0f;
    asteroid_make_new_default(x, y, direction, scale);

    // Child 2
    direction = asteroid->direction + ((rand()%101)-50.0f)/100.0f; // Some randomness inserted
    scale = asteroid->scale / 2.0f;
    x = asteroid->x + (rand()%100) - 50.0f;
    y = asteroid->y + (rand()%100) - 50.0f;
    asteroid_make_new_default(x, y, direction, scale);

    asteroid_delete(asteroid);
}

/**
 * @brief      Calculates the speed of an asteroid
 *
 * @param      asteroid  The asteroid
 *
 * @return     Speed value
 */
float asteroid_calc_speed(Asteroid *asteroid) {
    return 5.0f - asteroid->scale;
}

/**
 * @brief      Checks for collision between the asteroid and the ship
 *
 * @param      asteroid  The asteroid
 * @param      ship      The ship
 *
 * @return     true for collision; false otherwise
 */
bool asteroid_check_collision_on_ship(Asteroid *asteroid, Ship *ship) {
    // Ship's base points
    float x[7];
    float y[7];

    // Asteroid's corners
    float x1;
    float y1;
    float x2;
    float y2;

    // Auxiliar
    int32 i;

    // Get points
    ship_get_base_points(ship, x, y);
    asteroid_get_corners(asteroid, &x1, &y1, &x2, &y2);

    // Check for collision for each base point
    for (i = 0; i < 7; ++i) {
        if (common_check_collision(x[i], y[i], x1, y1, x2, y2)) {
            return true;
        }
    }    

    return false;
}
