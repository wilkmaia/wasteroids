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

#include "wasteroids.h"

// Global variables definitions
struct ALLEGRO_DISPLAY *screen;
struct ALLEGRO_FONT *font;
struct ALLEGRO_FONT *font_video;


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

