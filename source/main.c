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
 * Main project file
 */

#define WAS_USING_INPUT
#define WAS_USING_HISCORE
#define WAS_USING_SHIP
#define WAS_USING_BLAST
#include "wasteroids.h"

 int main(int argc, char *argv[]) {
    int8 i;
    int32 display_flags;
    int32 n;
    int32 width;
    int32 height;

    srand(time(NULL));

    /*==========================================
    =            Initialise Allegro            =
    ==========================================*/
    al_set_org_name("Wilk Maia");
    al_set_app_name("WAsteroids");

    if (!al_init()) {
        error("Couldn't initialise Allegro");
    }
    
    if (!al_init_primitives_addon()) {
        error("Couldn't initialise Allegro Primitives Addon");
    }

    display_flags = ALLEGRO_GENERATE_EXPOSE_EVENTS;
    width = 0;
    height = 0;


    /*============================================
    =            Command-line parsing            =
    ============================================*/
    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--fullscreen") == 0) {
            display_flags |= ALLEGRO_FULLSCREEN;
        }
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage_message();
            return -1;
        }
        else {
            n = atoi(argv[i]);

            if (!n) {
                print_usage_message();
                return -1;
            }

            if (0 == width) {
                width = n;
            }
            else if (0 == height) {
                height = n;
            }
            else {
                print_usage_message();
                return -1;
            }
        }
    }


    /*=========================================
    =            New Display Setup            =
    =========================================*/
    // If user didn't input width, height or fullcreen option
    // Defaults to 800x600
    if (width == 0 || height == 0
            || !(display_flags & ALLEGRO_FULLSCREEN)) {
        width = 800;
        height = 600;
    }

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_SUGGEST);

    al_set_new_display_flags(display_flags);

    screen = al_create_display(width, height);
    if (!screen) {
        char buf[128] = {};
        sprintf(buf, "Error setting %dx%d display mode", width, height);
        error(buf);
    }


    /*====================================
    =            Other setups            =
    ====================================*/
    al_install_keyboard();

    input_init();
    hiscore_init();


    /*====================================
    =            Game objects            =
    ====================================*/
    Ship *ship = (Ship *)malloc(sizeof(Ship));
    ship = ship_make_new_default();


    /*=================================
    =            Game loop            =
    =================================*/
    while (run_game(ship))
    {
        
    }


    /*============================================
    =            Game objects cleanup            =
    ============================================*/
    ship_delete(ship);
    blast_delete_all();


    /*=======================================
    =            Allegro Cleanup            =
    =========================================*/
    hiscore_shutdown();
    input_shutdown();

    // al_destroy_font(font);
    // al_destroy_font(font_video);


    return 0;
 }
