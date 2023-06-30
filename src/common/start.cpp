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

    ICMCProcessor icmc(icmc_ram);
    icmc.run();

    int cnt = 0;
    while (endless_loop_running) {
        uint32_t current_char = read_char();
        if (current_char != SENTINEL_KEYPRESSED_VALUE) {
            show_char(current_char & 0xff, cnt++, 0x0f0, 0x00f);
        }
        sleep_us(1);
    }
}