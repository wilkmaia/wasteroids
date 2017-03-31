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
 * Text functions
 */

#define WAS_USING_TEXT
#include "wasteroids.h"


/*=========================================
=            Local Definitions            =
=========================================*/

/**
 * @brief       Copies content from string src to dst with a maximum length of MAX_LENGTH
 * @param       dst         Destination string
 * @param       src         Source string
 * @param       MAX_LENGTH  Maximum length to be copied
 */
void myStrCpy(char *dst, const char *src, size_t MAX_LENGTH) {
    size_t length = strlen(src);
    if (length > MAX_LENGTH) {
        length = MAX_LENGTH;
    }

    memcpy(dst, src, length);
    dst[length] = 0;
}

/*=====  End of Local Definitions  ======*/



/**
 * @brief      Creates a new text element
 *
 * @param[in]  font   The font
 * @param[in]  scale  The scale
 * @param[in]  x      absolute x coord on screen
 * @param[in]  y      absolute y coord on screen
 * @param[in]  color  The color
 * @param[in]  msg    The message to print
 *
 * @return     Pointer to new text element
 */
text * text_make_new(const char *font, float scale, float x, float y, ALLEGRO_COLOR color, const char *msg) {
    text *newText = (text *)malloc(sizeof(text));

    newText->font = al_load_font(font, 0, 0);
    newText->scale = scale;
    newText->x = x;
    newText->y = y;
    newText->color = color;
    myStrCpy(newText->msg, msg, TEXT_MESSAGE_LENGTH - 1);

    return newText;
}

/**
 * @brief      Creates a new text element with default font and color
 *
 * @param[in]  scale  The scale
 * @param[in]  x      absolute x coord on screen
 * @param[in]  y      absolute y coord on screen
 * @param[in]  msg    The message to print
 *
 * @return     Pointer to new text element
 */
text * text_make_new_default(float scale, float x, float y, const char *msg) {
    return text_make_new("fonts/a4_font.tga", scale, x, y, al_map_rgb(255, 255, 255), msg);
}

/**
 * @brief      Frees memory occupied by text element
 *
 * @param      t     Pointer to text element
 *
 * @return     NULL in case of success
 */
text * text_delete(text *t) {
    al_destroy_font(t->font);
    free(t);
    t = NULL;

    return t;
}


/**
 * @brief      Draws text on scren
 *
 * @param      t     Text element to be drawn
 */
void text_draw(text *t) {
    ALLEGRO_TRANSFORM transform;
    const ALLEGRO_TRANSFORM * prevTransform;

    // Saves current transform state
    prevTransform = al_get_current_transform();

    // Transforms based on asteroid info
    al_identity_transform(&transform);
    al_scale_transform(&transform, t->scale, t->scale);
    al_translate_transform(&transform, 0, 0);
    al_use_transform(&transform);

    al_draw_text(t->font, al_map_rgb(255, 255, 255), t->x/t->scale, t->y/t->scale, 0, t->msg);

    if (prevTransform != NULL) {
        al_use_transform(prevTransform);
    }
}


/**
 * @brief       Updates message on text element t
 * @param       t       Text element to be updated
 * @param       msg     New message
 */
void text_update_msg(text *t, const char *msg) {
    myStrCpy(t->msg, msg, TEXT_MESSAGE_LENGTH - 1);
}
