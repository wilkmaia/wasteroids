#pragma once
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

/*=================================================
=            Standard Library includes            =
=================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>

/*=====  End of Standard Library includes  ======*/



/*===============================
=            Allegro            =
===============================*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

/*=====  End of Allegro  ======*/


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/*=========================================
=            Default datatypes            =
=========================================*/

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#ifndef NULL
    #define NULL (void *)0
#endif // NULL

#ifndef NUL
    #define NUL 0
#endif // NUL

#ifndef FALSE
    #define FALSE (0 != 0)
#endif // FALSE

#ifndef TRUE
    #define TRUE (0 == 0)
#endif // TRUE

/*=====  End of Default datatypes  ======*/



/*=============================================
=            WAsteroids' specifics            =
=============================================*/

// Global variables

/**
 * @brief      screen handler
 */
extern struct ALLEGRO_DISPLAY *screen;

/**
 * @brief       key map
 */
extern int32 key[ALLEGRO_KEY_MAX];

/**
 * @brief      font struct
 */
extern struct ALLEGRO_FONT *font;

/**
 * @brief      video font struct
 */
extern struct ALLEGRO_FONT *font_video;


// Ship specifics
#ifdef WAS_USING_SHIP
typedef struct {
    float x;
    float y;
    float direction;
    float speed;
    bool alive;
    ALLEGRO_COLOR color;
} Ship;


Ship * ship_make_new(float x, float y, float direction, float speed,
                   bool alive, ALLEGRO_COLOR color);
int8 ship_draw(Ship *ship);
Ship * ship_delete(Ship *ship);
#endif // WAS_USING_SHIP


// Input specifics
#ifdef WAS_USING_INPUT

void input_init();
void input_shutdown();
#endif // WAS_USING_INPUT


// Hiscore specifics
#ifdef WAS_USING_HISCORE
#define NUM_SCORES 8
#define MAX_NAME_LEN 24

void hiscore_init();
void hiscore_shutdown();
#endif // WAS_USING_HISCORE
/*=====  End of WAsteroids' specifics  ======*/



/*==================================================
=            Common function prototypes            =
==================================================*/

/**
 * @brief      Displays error message before terminating program
 *
 * @param      msg   Error message
 */
void error(char *msg);

/**
 * @brief      Prints usage message
 */
void print_usage_message();

/**
 * @brief      Gets configuration value (number)
 *
 * @param[in]  cfg      The configuration file
 * @param[in]  section  The section
 * @param[in]  name     The name
 * @param[in]  def      The definition
 *
 * @return     The configuration value (number)
 */
int32 get_config_int(const ALLEGRO_CONFIG *cfg, const char *section, 
                     const char *name, int32 def);

/**
 * @brief      Gets the configuration value (text)
 *
 * @param[in]  cfg      The configuration file
 * @param[in]  section  The section
 * @param[in]  name     The name
 * @param[in]  def      The definition
 *
 * @return     The configuration value (text)
 */
const char *get_config_string(const ALLEGRO_CONFIG *cfg, const char *section,
                              const char *name, const char *def);

/**
 * @brief      Sets the configuration value (number)
 *
 * @param      cfg      The configuration
 * @param[in]  section  The section
 * @param[in]  name     The name
 * @param[in]  val      The value
 */
void set_config_int(ALLEGRO_CONFIG *cfg, const char *section,
                    const char *name, int32 val);

/**
 * @brief      Sets the configuration value (text).
 *
 * @param      cfg      The configuration
 * @param[in]  section  The section
 * @param[in]  name     The name
 * @param[in]  val      The value
 */
void set_config_string(ALLEGRO_CONFIG *cfg, const char *section,
                              const char *name, const char *val);

/*=====  End of Common function prototypes  ======*/

#ifdef __cplusplus
}
#endif // __cplusplus
