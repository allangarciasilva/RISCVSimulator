#include "common/processor_api.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "simulated/RISCVSimulator.h"

extern "C" {
void sleep_us(uint32_t us) {
    std::this_thread::sleep_for(std::chrono::microseconds(us));
}

void set_render_buffer_address(uint32_t *address) {
    RISCVSimulator::render_buffer = address;
}

uint32_t read_char() {
    auto tmp = simulator.keypressed_buffer;
    simulator.keypressed_buffer = SENTINEL_KEYPRESSED_VALUE;
    return tmp;
}
}