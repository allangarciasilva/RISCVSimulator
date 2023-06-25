#pragma once

#include <SDL.h>

#include "common/processor_api.h"

#define VGA_WIDTH 640
#define VGA_HEIGHT 480
#define VGA_AMPLIFICATION 2

#define WIN_WIDTH (VGA_WIDTH * 2)
#define WIN_HEIGHT (VGA_HEIGHT * 2)


struct Color {
    uint8_t r, g, b;

    static Color from_12_bit_binary(uint16_t value);
};

class RISCVSimulator {
   public:
    static uint32_t *render_buffer;

    explicit RISCVSimulator(const char *charmap_file);
    ~RISCVSimulator();

    void run();
    int keypressed_buffer = SENTINEL_KEYPRESSED_VALUE;

   private:
    SDL_Window   *window   = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture  *texture  = nullptr;
    SDL_Surface  *surface  = nullptr;
    bool         halted    = false;

    uint8_t pixels[WIN_WIDTH * WIN_HEIGHT * 4] = {};
    bool charmap_buffer[N_CHARS][64] = {};

    void load_charmap(const char *charmap_file);

    Color get_pixel_color(uint16_t vga_i, uint16_t vga_j);
    void render_screen();
};

extern RISCVSimulator simulator;