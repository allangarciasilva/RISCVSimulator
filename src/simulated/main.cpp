#include <iostream>
#include <thread>

#include "common/processor_api.h"
#include "simulated/RISCVSimulator.h"

bool endless_loop_running = true;

int main() {
    std::thread render_thread(start);

    simulator.run();
    endless_loop_running = false;

    render_thread.join();
}