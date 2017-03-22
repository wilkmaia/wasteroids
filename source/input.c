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
 * Input functions
 */

#define WAS_USING_INPUT
#include "wasteroids.h"


// Local global variables
static int32 keybuf_len = 0;
static ALLEGRO_MUTEX *keybuf_mutex;
static ALLEGRO_EVENT_QUEUE *input_queue;
static ALLEGRO_TIMER *timer;
static float FPS = 60.0f;

/**
 * @brief      Initialise input service
 */
void input_init() {
    keybuf_len = 0;
    keybuf_mutex = al_create_mutex();
    
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        error("Failed to create timer");
    }
    al_start_timer(timer);

    // Registers input queue
    input_queue = al_create_event_queue();
    al_register_event_source(input_queue, al_get_keyboard_event_source());
    al_register_event_source(input_queue, al_get_display_event_source(screen));
    al_register_event_source(input_queue, al_get_timer_event_source(timer));
}

/**
 * @brief      Cleanup for input service
 */
void input_shutdown() {
    al_destroy_mutex(keybuf_mutex);
    keybuf_mutex = NULL;

    al_destroy_event_queue(input_queue);
    input_queue = NULL;

    al_destroy_timer(timer);
    timer = NULL;
}

/**
 * @brief      Waits for an event and registers it in *ev
 *
 * @param      ev    pointer to ALLEGRO_EVENT
 */
void input_wait_for_event(ALLEGRO_EVENT *ev) {
    al_wait_for_event(input_queue, ev);
}

/**
 * @brief      Checks if event queue is empty
 *
 * @return     true for empty queue; false otherwise
 */
bool input_is_queue_empty() {
    return al_is_event_queue_empty(input_queue);
}
