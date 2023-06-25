#include <iostream>
#include <thread>

#include "common/processor_api.h"
#include "simulated/RISCVSimulator.h"

int main() {
    std::thread render_thread([]() { simulator.run(); });

    start();

    render_thread.join();
}