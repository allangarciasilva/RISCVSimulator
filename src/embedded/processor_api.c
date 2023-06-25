#include "common/processor_api.h"

inline void sleep_ms(uint32_t ms) {
    unsigned us = ms * 1000;
    asm volatile(
        "li a7, %[ecall_number]\n\t"
        "mv a0, %[operand]\n\t"
        "ecall\n\t"
        :
        : [ecall_number] "i"(3), [operand] "r"(us)
        : "a7", "a0");
}

inline void set_render_buffer_address(uint32_t* address) {
    asm volatile(
        "li a7, %[ecall_number]\n\t"
        "mv a0, %[operand]\n\t"
        "ecall\n\t"
        :
        : [ecall_number] "i"(1), [operand] "r"(address)
        : "a7", "a0", "memory");
}

inline uint32_t read_char() {
    int result;

    asm volatile(
        "li a7, %[ecall_number]\n\t"
        "ecall\n\t"
        "mv %[result], a0\n\t"
        : [result] "=r"(result)
        : [ecall_number] "i"(2)
        : "a7", "a0");

    return result;
}

bool endless_loop_running = true;