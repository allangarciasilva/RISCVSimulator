#pragma once

#include <stdint.h>

#include "common/ICMC/ICMCConstants.h"
#include "common/ICMC/bit_operations.h"
#include "common/processor_api.h"

class ICMCProcessor {
   public:
    static const uint16_t REGISTER_MAX_VALUE = UINT16_MAX;
    static const uint16_t ROM_SIZE = 32 * 1024;

    ICMCProcessor(uint16_t* memory);
    void run();

   private:
    bool halt = false;
    uint16_t keypressed_buffer = SENTINEL_KEYPRESSED_VALUE;

    uint16_t* memory;

    uint16_t registers[8] = {};
    uint16_t stack_pointer = ICMCProcessor::ROM_SIZE - 1;
    uint16_t program_counter = 0;
    uint16_t instruction_register = 0;
    uint16_t flag_register = 0;

    uint16_t fetch_next_word();
    void execute_next_instruction();

    inline uint16_t& rx();
    inline uint16_t& ry();
    inline uint16_t& rz();
    inline uint8_t carry();

    inline bool get_flag(ICMCFlag flag) const;
    inline void set_flag(ICMCFlag flag, bool value);
    bool should_jump();

    void reset_arithmetic_flags(uint32_t value);
    void reset_logic_flags();

    void store_alu(uint32_t value);
    void store_add(uint32_t a, uint32_t b);
    void store_sub(uint32_t a, uint32_t b);

    // Instruções de manipulação de dados
    inline void execute_store();
    inline void execute_load();
    inline void execute_storei();
    inline void execute_loadi();
    inline void execute_loadn();
    inline void execute_mov();

    // Instruções aritméticas
    inline void execute_add();
    inline void execute_sub();
    inline void execute_mult();
    inline void execute_div();
    inline void execute_incdec();
    inline void execute_mod();

    // Instruções lógicas
    inline void execute_and();
    inline void execute_or();
    inline void execute_xor();
    inline void execute_not();
    inline void execute_shift();
    inline void execute_cmp();

    // Instruções de entrada e saída
    inline void execute_inchar();
    inline void execute_outchar();

    // Instruções de salto e chamada
    inline void execute_jmp();
    inline void execute_call();
    inline void execute_rts();

    // Instruções de chamada
    inline void execute_push();
    inline void execute_pop();

    // Instruções de controle
    inline void execute_carry();
    inline void execute_halt();
    inline void execute_noop();
    inline void execute_breakp();
};
