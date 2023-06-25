#include "common/processor_api.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "simulated/RISCVSimulator.h"

extern "C" {
void sleep_ms(uint32_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void set_render_buffer_address(uint32_t *address) {
    RISCVSimulator::render_buffer = address;
}

uint32_t read_char() {
    uint32_t current_char = simulator.keypressed_buffer;
    simulator.keypressed_buffer = SENTINEL_KEYPRESSED_VALUE;
    return current_char;
}
}