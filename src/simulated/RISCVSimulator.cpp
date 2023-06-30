#include "RISCVSimulator.h"

#include <iostream>
#include <stdexcept>

#include "common/processor_api.h"

uint32_t *RISCVSimulator::render_buffer = nullptr;

RISCVSimulator::RISCVSimulator(const char *charmap_file) {
    // SDL init
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        throw;
    }

    // create SDL window
    window = SDL_CreateWindow("sdl2_pixelbuffer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH,
                              WIN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        throw;
    }

    // create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        throw;
    }

    SDL_RenderSetLogicalSize(renderer, WIN_WIDTH, WIN_HEIGHT);

    // create texture
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WIN_WIDTH,
                                WIN_HEIGHT);
    if (texture == NULL) {
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        throw;
    }

    load_charmap(charmap_file);
}

void RISCVSimulator::load_charmap(const char *charmap_file) {
    FILE *charmap_fp = fopen(charmap_file, "rb");
    if (charmap_fp == NULL) {
        return;
    }
    fread(charmap_buffer, sizeof(charmap_buffer), 1, charmap_fp);
    fclose(charmap_fp);
}

RISCVSimulator::~RISCVSimulator() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void RISCVSimulator::run() {
    bool should_quit = false;
    SDL_Event e;
    while (!should_quit) {
        render_screen();

        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    should_quit = true;
                    break;
                case SDL_KEYDOWN:
                    keypressed_buffer = e.key.keysym.sym;
                    break;
            }
        }

        int texture_pitch = 0;
        void *texture_pixels = NULL;
        if (SDL_LockTexture(texture, NULL, &texture_pixels, &texture_pitch) != 0) {
            SDL_Log("Unable to lock texture: %s", SDL_GetError());
        } else {
            memcpy(texture_pixels, pixels, texture_pitch * WIN_HEIGHT);
        }
        SDL_UnlockTexture(texture);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}

#define debug(x) #x << "=" << (int)x << " "

Color RISCVSimulator::get_pixel_color(uint16_t vga_x, uint16_t vga_y) {
    uint16_t char_y = (vga_y / VGA_AMPLIFICATION) / 8;
    uint16_t char_x = (vga_x / VGA_AMPLIFICATION) / 8;
    uint16_t tile_idx = char_x + char_y * SCREEN_WIDTH;
    uint32_t tile_data = 0;
    if (render_buffer != nullptr and tile_idx < SCREEN_SIZE) {
        tile_data = render_buffer[tile_idx];
    }

    bool *character = charmap_buffer[tile_data & 0xff];
    uint16_t color = (tile_data >> 20) & 0xfff;
    uint16_t bg_color = (tile_data >> 8) & 0xfff;

    uint8_t pix_y = (vga_y / VGA_AMPLIFICATION) % 8;
    uint8_t pix_x = (vga_x / VGA_AMPLIFICATION) % 8;
    uint8_t pixel_idx = (7 - pix_x) + pix_y * 8;

    bool is_foreground = pixel_idx < N_CHARS && character[pixel_idx];
    uint16_t selected_color = is_foreground ? color : bg_color;

    return Color::from_12_bit_binary(selected_color);
}

void RISCVSimulator::render_screen() {
    for (int y = 0; y < WIN_HEIGHT; y++) {
        for (int x = 0; x < WIN_WIDTH; x++) {
            uint16_t vga_x = x / (WIN_WIDTH / VGA_WIDTH);
            uint16_t vga_y = y / (WIN_HEIGHT / VGA_HEIGHT);

            Color pixel_color = get_pixel_color(vga_x, vga_y);
            int pix = y * WIN_WIDTH + x;

            pixels[pix * 4 + 0] = pixel_color.r;
            pixels[pix * 4 + 1] = pixel_color.g;
            pixels[pix * 4 + 2] = pixel_color.b;
            pixels[pix * 4 + 3] = 255;
        }
    }
}

Color Color::from_12_bit_binary(uint16_t value) {
    static const uint8_t multiplier = 0xff / 0xf;
    uint8_t r = multiplier * ((value >> 8) & 0xf);
    uint8_t g = multiplier * ((value >> 4) & 0xf);
    uint8_t b = multiplier * ((value >> 0) & 0xf);
    return Color{r, g, b};
}

#ifndef CHARMAP_FILE
#define CHARMAP_FILE ""
#endif

RISCVSimulator simulator(CHARMAP_FILE);