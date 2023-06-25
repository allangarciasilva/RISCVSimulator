#include "ICMC/ICMCProcessor.h"
#include "ICMC/icmc_ram.h"
#include "processor_api.h"

uint32_t buffer[SCREEN_SIZE];

void show_char(uint8_t character, uint16_t position, uint16_t foreground_color, uint16_t background_color) {
    if (position >= SCREEN_SIZE) {
        return;
    }
    if (character >= N_CHARS) {
        character = N_CHARS - 1;
    }
    foreground_color &= 0xfff;
    background_color &= 0xfff;
    buffer[position] = (foreground_color << 20) | (background_color << 8) | character;
}

void show_text(const char *text, uint16_t position, uint16_t foreground_color, uint16_t background_color) {
    for (int i = 0; text[i] != '\0'; i++) {
        show_char(text[i], position + i, foreground_color, background_color);
    }
}

extern "C" void start() {
    set_render_buffer_address(buffer);

    // show_text("Allan", 0, 0xfff, 0x000);
    // show_text("Natan", 0, 0xfff, 0x000);
    // show_text("Simoes", 0, 0xfff, 0x000);

    // while (true) {
    //     show_text("Natan ", 0, 0xf00, 0x000);
    //     sleep_ms(500);

    //     show_text("Simoes", 0, 0x0f0, 0x000);
    //     sleep_ms(500);

    //     show_text("Allan ", 0, 0x00f, 0x000);
    //     sleep_ms(500);
    // }

    // while (true) {
    //     uint32_t current_char = read_char();
    //     if (current_char == SENTINEL_KEYPRESSED_VALUE) {
    //         current_char = '!';
    //     }
    //     show_char(current_char & 0xff, 0, 0xfff, 0);
    // }

    // char text[] = "Allan";

    // while (true) {
    //     for (char i = 0; text[i]; i++) {
    //         show_char(text[i], i, 0xfff, 0xf00);
    //         sleep_ms(16);
    //     }
    //     for (char i = 0; text[i]; i++) {
    //         show_char(' ', i, 0xfff, 0);
    //         sleep_ms(16);
    //     }
    // }

    ICMCProcessor icmc(icmc_ram);
    icmc.run();
}