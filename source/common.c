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
#include "wasteroids.h"

// Project variables definitions
struct ALLEGRO_DISPLAY *screen;
struct ALLEGRO_FONT *font;
struct ALLEGRO_FONT *font_video;
bool pressed_keys[ALLEGRO_KEY_MAX];
Blast *(blasts[WAS_MAX_BLASTS]);
int32 num_blasts = 0;


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

bool run_game(Ship *ship) {
    ALLEGRO_EVENT ev;
    input_wait_for_event(&ev);
    bool redraw = false;

    Blast *newBlast;

    // Checks for key pressed
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                return false;
                break;

            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_DOWN:
            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_RIGHT:
                pressed_keys[ev.keyboard.keycode] = true;
                break;

            case ALLEGRO_KEY_SPACE:
                if (num_blasts >= WAS_MAX_BLASTS)
                    break;

                newBlast = blast_make_new_default(ship->x, ship->y, ship->direction);
                break;

            default:
                break;
        }
    }
    // Check for key released
    else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_DOWN:
            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_RIGHT:
                pressed_keys[ev.keyboard.keycode] = false;
                break;
        }
    }
    // Checks for timer event
    else if (ev.type == ALLEGRO_EVENT_TIMER) {
        // TODO Run game logic
        ship_move(ship);
        blast_move_all();
        redraw = true;
    }

    if (redraw && input_is_queue_empty()) {
        int32 i;

        redraw = false;

        // Redraws objects on screen
        al_clear_to_color(al_map_rgb(0, 0, 0));
        
        ship_draw(ship);

        for (i = 0; i < num_blasts; ++i) {
            blast_draw(blasts[i]);
        }

        al_flip_display();
    }

    return true;
}

