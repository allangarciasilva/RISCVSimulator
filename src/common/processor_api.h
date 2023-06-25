#pragma once

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#define SENTINEL_KEYPRESSED_VALUE 255

#define N_CHARS 128
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 30
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

typedef signed char char_t;
typedef unsigned char uchar_t;

void start();
void sleep_ms(uint32_t ms);
void set_render_buffer_address(uint32_t* address);
uint32_t read_char();
void show_char(uint8_t character, uint16_t position, uint16_t foreground_color, uint16_t background_color);

extern bool endless_loop_running;

#ifdef __cplusplus
}
#endif