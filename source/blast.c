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
 * Blast functions
 * 
 * TODO
 */

#define WAS_USING_BLAST
#define WAS_USING_SHIP
#include "wasteroids.h"


/*=========================================
=            Local definitions            =
=========================================*/

static void print_coords(float x1, float y1, float x2, float y2) {
    printf("\nCoords:\n");
    printf("(x1, y1) = (%.2f, %.2f)\n", x1, y1);
    printf("(x2, y2) = (%.2f, %.2f)\n", x2, y2);
    printf("\n");
}

/*=====  End of Local definitions  ======*/


/**
 * @brief      Creates a new blast
 *
 * @param[in]  x          x position
 * @param[in]  y          y position
 * @param[in]  direction  The direction
 * @param[in]  speed      The speed
 * @param[in]  alive      The alive
 * @param[in]  color      The color
 *
 * @return     Pointer to new blast
 */
Blast * blast_make_new(float x, float y, float direction, float size, float speed,
                       bool alive, ALLEGRO_COLOR color, float thickness) {
    Blast * newBlast;
    newBlast = (Blast *) malloc(sizeof(Blast));

    // Value checking
    newBlast->x = x;
    newBlast->y = y;
    newBlast->direction = direction;
    newBlast->size = size;
    newBlast->speed = speed;
    newBlast->alive = alive;
    newBlast->color = color;
    newBlast->thickness = thickness;

    // Add new blast to blast list
    blasts[num_blasts] = newBlast;
    ++num_blasts;

    return newBlast;
}

/**
 * @brief      Creates a new blast with default values
 *
 * @param[in]  x          starting x-coordinate
 * @param[in]  y          starting y-coordinate
 * @param[in]  direction  starting direction
 *
 * @return     Pointer to new blast
 */
Blast * blast_make_new_default(float x, float y, float direction) {
    Blast * newBlast;
    float size = 20.0f;
    float speed = 10.0f;
    bool alive = true;
    ALLEGRO_COLOR color = BLAST_COLOR;
    float thickness = 3.0f;

    newBlast = blast_make_new(x, y, direction, size, speed, alive,
                              color, thickness);

    return newBlast;
}

/**
 * @brief      Draws blast on screen
 *
 * @param      blast  The blast
 *
 * @return     0 for success or anything else for error
 */
int8 blast_draw(Blast *blast) {
    // Blast initial and end points
    float x1;
    float y1;
    float x2;
    float y2;

    // Shouldn't draw if blast wasn't alive
    if (!blast->alive) {
        return -1;
    }

    x1 = blast->x;
    y1 = blast->y;
    blast_get_end_point(blast, &x2, &y2);

    // Draws the blast
    al_draw_line(x1, y1, x2, y2, blast->color, blast->thickness);

    return 0;
}

/**
 * @brief      Draws all active blasts to screen
 */
void blast_draw_all() {
    int32 i;

    for (i = 0; i < num_blasts; ++i) {
        blast_draw(blasts[i]);
    }
}

/**
 * @brief      Sets the end point coordinates of the blast on the addresses of the x- and y-variables
 *
 * @param      blast  The blast
 * @param[out] x      Address to end-point x-coordinate
 * @param[out] y      Address to end-point y-coordinate
 */
void blast_get_end_point(Blast *blast, float *x, float *y) {
    *x = blast->x + blast->size * cos(blast->direction);
    *y = blast->y - blast->size * sin(blast->direction);
}

/**
 * @brief      Deletes blast, freeing its memory
 *
 * @param      blast  The blast
 *
 * @return     Pointer to object. Should point to NULL if everything went ok.
 */
Blast * blast_delete(Blast *blast) {
    int32 i;

    // TODO
    // Implement a linked list instead maybe
    for (i = 0; i < num_blasts; ++i) {
        if (blasts[i] == blast) {
            break; // Found the element
        }
    }
    if (i < num_blasts) { // Element found
        int32 j;
        for (j = i + 1; j < num_blasts; ++j) {
            blasts[j-1] = blasts[j];
        }
        --num_blasts;
    }

    free(blast);
    blast = NULL;

    return blast;
}

/**
 * @brief      Delete all blasts on list
 */
void blast_delete_all() {
    int32 i;

    for (i = 0; i < num_blasts; ++i) {
        free(blasts[i]);
        blasts[i] = NULL;
    }

    num_blasts = 0;
}

/**
 * @brief      Move blast
 *
 * @param      blast  The blast
 */
void blast_move(Blast *blast) {
    float dx;
    float dy;

    int32 width;
    int32 height;

    // No need to waste time here if blast isn't alive
    if (!blast->alive) {
        blast_delete(blast);
        return;
    }

    width = al_get_display_width(screen);
    height = al_get_display_height(screen);

    // Check if out of bounds
    if (blast->x < 0 || blast->x > width 
            || blast->y < 0 || blast->y > height) {
        blast->alive = false;
        blast_delete(blast);
        return;
    }

    dx = blast->speed * cos(blast->direction);
    dy = - blast->speed * sin(blast->direction);

    blast->x += dx;
    blast->y += dy;
}

/**
 * @brief      Move all blasts
 */
void blast_move_all() {
    int32 i;

    for (i = 0; i < num_blasts; ++i) {
        blast_move(blasts[i]);
    }
}
