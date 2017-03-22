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
 * Functions to handle score
 */

#define WAS_USING_HISCORE
#include "wasteroids.h"


// Local global variables
static int scores[NUM_SCORES] = {
    666, 512, 440, 256, 192, 128, 64, 42
};
static char names[NUM_SCORES][MAX_NAME_LEN+1];


/**
 * @brief      Initialise hiscore service
 */
void hiscore_init() {
    ALLEGRO_PATH *path;
    ALLEGRO_CONFIG *cfg;
    char buf1[256];
    int32 i;

    path = al_get_standard_path(ALLEGRO_USER_DATA_PATH);
    if (!path) {
        error("No path");
    }

    // Creates High Score file
    al_make_directory(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
    al_set_path_filename(path, "highscore.rec");

    // Load configuration file or make one if it doesn't exist
    cfg = al_load_config_file(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
    if (!cfg) {
        cfg = al_create_config();
    }

    for (i = 0; i < NUM_SCORES; ++i) {
        sprintf(buf1, "score%d", i + 1);
        scores[i] = get_config_int(cfg, "hiscore", buf1, scores[i]);

        sprintf(buf1, "name%d", i + 1);
        strncpy(names[i], get_config_string(cfg, "hiscore", buf1, "Wilk Maia"), MAX_NAME_LEN);
        names[i][MAX_NAME_LEN] = 0;
    }

    al_destroy_config(cfg);
    al_destroy_path(path);
}

/**
 * @brief      Cleanup for hiscore service
 */
void hiscore_shutdown() {
    ALLEGRO_PATH *path;
    ALLEGRO_CONFIG *cfg;
    char buf1[256];
    int32 i;

    path = al_get_standard_path(ALLEGRO_USER_DATA_PATH);
    if (!path) {
        error("No path");
    }

    // Creates High Score file
    al_make_directory(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
    al_set_path_filename(path, "highscore.rec");

    // Load configuration file or make one if it doesn't exist
    cfg = al_load_config_file(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
    if (!cfg) {
        cfg = al_create_config();
    }

    for (i = 0; i < NUM_SCORES; ++i) {
        sprintf(buf1, "score%d", i + 1);
        set_config_int(cfg, "hiscore", buf1, scores[i]);

        sprintf(buf1, "name%d", i + 1);
        set_config_string(cfg, "hiscore", buf1, names[i]);
    }

    al_save_config_file(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP), cfg);

    al_destroy_config(cfg);
    al_destroy_path(path);
}
