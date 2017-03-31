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
 * Common functions
 */

#define WAS_USING_INPUT
#define WAS_USING_SHIP
#define WAS_USING_BLAST
#define WAS_USING_ASTEROID
#define WAS_USING_TEXT
#include "wasteroids.h"


/*==============================================================
=            Project global variables and constants            =
==============================================================*/

struct ALLEGRO_DISPLAY *screen;
struct ALLEGRO_FONT *font;
bool pressed_keys[ALLEGRO_KEY_MAX];

Ship *ship;

Blast *(blasts[BLAST_MAX]);
int32 num_blasts = 0;

Asteroid *(asteroids[ASTEROID_MAX]);
int32 num_asteroids = 0;

const float SHIP_DIMENSION = 20.0f;
const float ASTEROID_DIMENSION = 20.0f;

const float MAX_ANGLE = 2.0f * (float) ALLEGRO_PI;

const float VERTICES[] = {
    -20, 20,
    -25, 5,
    -25, -10,
    -5, -10,
    -10, -20,
    5, -20,
    20, -10,
    20, -5,
    0, 0,
    20, 10,
    10, 20,
    0, 15
};

bool is_game_over = false;

text *score = NULL;

uint32 score_count = 0;

/*=====  End of Project global variables and constants  ======*/



void error(char *msg) {
    fprintf(stderr, "%s: %s", msg, strerror(errno));
    exit(1);
}

void print_usage_message() {
    printf(
        "\n"
        "WAsteroids\n"
        "by Wilk Maia, 2017\n"
        "\n"
        "Usage: wasteroids width height [options]\n"
        "\n"
        "width and height set screen resolution\n"
        "Other options are:\n"
        "\t--fullscreen\tenables full screen mode (makes width and height optional)\n"
        "\t--help [-h]\tdisplays this message\n"
        "\n"
        "Example:\n"
        "\twasteroids 1024 768\n"
        "\twasteroids --fullscreen\n"
        "\n"
    );
}

int32 get_config_int(const ALLEGRO_CONFIG *cfg, const char *section, 
                     const char *name, int32 def) {
    const char * v = al_get_config_value(cfg, section, name);

    return (v) ? atoi(v) : def;
}

const char *get_config_string(const ALLEGRO_CONFIG *cfg, const char *section,
                              const char *name, const char *def) {
    const char *v = al_get_config_value(cfg, section, name);

    return (v) ? v : def;
}

void set_config_int(ALLEGRO_CONFIG *cfg, const char *section,
                    const char *name, int32 val) {
    char buf[32];
    sprintf(buf, "%d", val);

    al_set_config_value(cfg, section, name, buf);
}

void set_config_string(ALLEGRO_CONFIG *cfg, const char *section,
                              const char *name, const char *val) {
    al_set_config_value(cfg, section, name, val);
}

bool run_game() {
    ALLEGRO_EVENT ev;
    input_wait_for_event(&ev);
    bool redraw = false;

    // Checks for key pressed
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (ev.keyboard.keycode) {
            // Finishes the game
            case ALLEGRO_KEY_ESCAPE:
                return false;

            // Move ship around
            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_DOWN:
            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_RIGHT:
                pressed_keys[ev.keyboard.keycode] = true;
                break;

            // Fires blast
            case ALLEGRO_KEY_SPACE:
                if (num_blasts >= BLAST_MAX)
                    break;
                blast_make_new_default(ship->x, ship->y, ship->direction);
                break;

            default:
                break;
        }
    }
    // Check for key released
    else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        // Stop moving ship
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_DOWN:
            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_RIGHT:
                pressed_keys[ev.keyboard.keycode] = false;
                break;

            default:
                break;
        }
    }
    // Checks for timer event
    // If game is over, there's no update on screen
    else if (ev.type == ALLEGRO_EVENT_TIMER && !is_game_over) {
        // TODO Run game logic
        // Move objects around
        ship_move(ship);
        blast_move_all();
        asteroid_move_all();

        // Check for collision
        check_blasts_on_asteroids();
        check_ship_on_asteroids();

        if (!ship->can_be_hit) {
            ++(ship->can_be_hit_count);
            if (ship->can_be_hit_count >= 60) {
                ship->can_be_hit = true;
                ship->can_be_hit_count = 0;
            }
        }

        // Sets flag for screen redrawing
        redraw = true;
    }

    if (redraw && input_is_queue_empty()) {
        // Redraws objects on screen
        al_clear_to_color(al_map_rgb(0, 0, 0));
        
        ship_draw(ship);
        blast_draw_all();
        asteroid_draw_all();
        text_draw(score);

        al_flip_display();
    }

    return true;
}

bool common_check_collision(float x, float y, float corner_x1, float corner_y1,
                            float corner_x2, float corner_y2) {
    if (x >= corner_x1 && y >= corner_y1
            && x <= corner_x2 && y <= corner_y2) {
        return true;
    }

    return false;
}

void check_blasts_on_asteroids() {
    int32 i;

    // For each blast
    for (i = 0; i < num_blasts; ++i) {
        // Check collision on asteroids
        int32 j;
        for (j = 0; j < num_asteroids; ++j) {
            // If they collide
            if (asteroid_check_collision_on_blast(asteroids[j], blasts[i])) {
                char msg[TEXT_MESSAGE_LENGTH] = {};

                // Handle the asteroid collision
                asteroid_was_hit(asteroids[j]);

                // Deletes blast
                blast_delete(blasts[i]);

                // Increases score
                score_count += 100;
                sprintf(msg, "Score: %d", score_count);
                text_update_msg(score, msg);
            }
        }
    }
}

void check_ship_on_asteroids() {
    int32 i;
    int8 lives;

    // For each asteroid
    for (i = 0; i < num_asteroids; ++i) {
        if (ship->can_be_hit && asteroid_check_collision_on_ship(asteroids[i], ship)) {
            lives = ship_hit(ship);

            // Checks for game over
            if (lives <= 0) {
                game_over();
            }
        }
    }
}

void game_over() {
    is_game_over = true;
}
