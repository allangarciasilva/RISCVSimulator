#include "ICMCProcessor.h"

#include "common/processor_api.h"

void ICMCProcessor::execute_next_instruction() {
    instruction_register = fetch_next_word();
    auto opcode = slice(instruction_register, 10, 15);

    // clang-format off
    switch (opcode) {
        // Instruções de manipulação de dados
        case ICMCOpcode::STORE:     return execute_store();
        case ICMCOpcode::LOAD:      return execute_load();
        case ICMCOpcode::STOREI:    return execute_storei();
        case ICMCOpcode::LOADI:     return execute_loadi();
        case ICMCOpcode::LOADN:     return execute_loadn();
        case ICMCOpcode::MOV:       return execute_mov();

        // Instruções aritméticas
        case ICMCOpcode::ADD:       return execute_add();
        case ICMCOpcode::SUB:       return execute_sub();
        case ICMCOpcode::MULT:      return execute_mult();
        case ICMCOpcode::DIV:       return execute_div();
        case ICMCOpcode::INCDEC:    return execute_incdec();
        case ICMCOpcode::MOD:       return execute_mod();

        // Instruções lógicas
        case ICMCOpcode::AND:       return execute_and();
        case ICMCOpcode::OR:        return execute_or();
        case ICMCOpcode::XOR:       return execute_xor();
        case ICMCOpcode::NOT:       return execute_not();
        case ICMCOpcode::SHIFT:     return execute_shift();
        case ICMCOpcode::CMP:       return execute_cmp();

        // Instruções de entrada e saída
        case ICMCOpcode::INCHAR:    return execute_inchar();
        case ICMCOpcode::OUTCHAR:   return execute_outchar();

        // Instruções de salto e chamada
        case ICMCOpcode::JMP:       return execute_jmp();
        case ICMCOpcode::CALL:      return execute_call();
        case ICMCOpcode::RTS:       return execute_rts();

        // Instruções de chamada
        case ICMCOpcode::PUSH:      return execute_push();
        case ICMCOpcode::POP:       return execute_pop();

        // Instruções de controle
        case ICMCOpcode::CARRY:     return execute_carry();
        case ICMCOpcode::HALT:      return execute_halt();
        case ICMCOpcode::NOOP:      return execute_noop();
        case ICMCOpcode::BREAKP:    return execute_breakp();

        default:                    return;
    }
    // clang-format on
}

// Instruções de manipulação de dados
void ICMCProcessor::execute_store() {
    auto addr = fetch_next_word();
    memory[addr] = rx();
}

void ICMCProcessor::execute_load() {
    auto addr = fetch_next_word();
    rx() = memory[addr];
}

void ICMCProcessor::execute_storei() {
    memory[rx()] = ry();
}

void ICMCProcessor::execute_loadi() {
    rx() = memory[ry()];
}

void ICMCProcessor::execute_loadn() {
    auto content = fetch_next_word();
    rx() = content;
}

void ICMCProcessor::execute_mov() {
    if (slice(instruction_register, 0, 0) == 0) {
        rx() = ry();
    } else {
        auto selection = slice(instruction_register, 0, 1);
        if (selection == 0b01) {
            rx() = stack_pointer;
        } else if (selection == 0b11) {
            stack_pointer = rx();
        }
    }
}

// Instruções aritméticas
void ICMCProcessor::execute_add() {
    store_add(ry(), rz());
}

void ICMCProcessor::execute_sub() {
    store_sub(ry(), rz());
}

void ICMCProcessor::execute_mult() {
    uint32_t y = ry(), z = rz();
    uint32_t value = y * z;

    if (value > REGISTER_MAX_VALUE) {
        set_flag(ICMCFlag::OVERFLOW, true);
        value = value % REGISTER_MAX_VALUE;
    }

    store_alu(value);
}

void ICMCProcessor::execute_div() {
    uint32_t y = ry(), z = rz();

    uint32_t value;
    if (z == 0) {
        set_flag(ICMCFlag::DIV_BY_ZERO, true);
        value = REGISTER_MAX_VALUE;
    } else {
        value = y / z;
    }

    store_alu(value);
}

void ICMCProcessor::execute_incdec() {
    uint32_t x = rx();
    if (slice(instruction_register, 6, 6) == 0) {
        store_add(rx(), 1);
    } else {
        store_sub(rx(), 1);
    }
}

void ICMCProcessor::execute_mod() {
    uint32_t y = ry(), z = rz();
    store_alu(y % z);
}

// Instruções lógicas
void ICMCProcessor::execute_and() {
    uint32_t y = ry(), z = rz();
    store_alu(y & z);
}

void ICMCProcessor::execute_or() {
    uint32_t y = ry(), z = rz();
    store_alu(y | z);
}

void ICMCProcessor::execute_xor() {
    uint32_t y = ry(), z = rz();
    store_alu(y ^ z);
}

void ICMCProcessor::execute_not() {
    uint16_t value = ~ry();
    store_alu(value);
}

void ICMCProcessor::execute_shift() {
    uint8_t n = slice(instruction_register, 0, 4);
    uint16_t value = rx();

    auto selection = slice(instruction_register, 5, 6);
    if (selection == 0b10) {
        value = rotl(rx(), n);
    } else if (selection == 0b11) {
        value = rotr(rx(), n);
    } else {
        selection = slice(instruction_register, 4, 6);
        if (selection == 0b000) {
            value = shiftl0(rx(), n);
        } else if (selection == 0b001) {
            value = shiftl1(rx(), n);
        } else if (selection == 0b010) {
            value = shiftr0(rx(), n);
        } else if (selection == 0b011) {
            value = shiftr1(rx(), n);
        }
    }

    store_alu(value);
}

void ICMCProcessor::execute_cmp() {
    uint16_t x = rx(), y = ry();

    reset_logic_flags();
    if (x > y) {
        set_flag(ICMCFlag::GREATER, true);
    } else if (x < y) {
        set_flag(ICMCFlag::LESS, true);
    } else {
        set_flag(ICMCFlag::EQUAL, true);
    }
}

// Instruções de entrada e saída
void ICMCProcessor::execute_inchar() {
    rx() = read_char() & 0xffff;
}

void ICMCProcessor::execute_outchar() {
    uint16_t character = rx();
    uint16_t position = ry();

    uint16_t color = ICMCColors::from_character(character);
    show_char(character & 0xff, position, color, ICMCColors::HEX_BLACK);
}

// Instruções de salto e chamada
bool ICMCProcessor::should_jump() {
    auto code = slice(instruction_register, 6, 9);

    // clang-format off
    switch (code) {
        case ICMCComparisonCode::ALWAYS:            return true;
        case ICMCComparisonCode::EQUAL:             return  get_flag(ICMCFlag::EQUAL);
        case ICMCComparisonCode::NOT_EQUAL:         return !get_flag(ICMCFlag::EQUAL);
        case ICMCComparisonCode::ZERO:              return  get_flag(ICMCFlag::ZERO);
        case ICMCComparisonCode::NOT_ZERO:          return !get_flag(ICMCFlag::ZERO);
        case ICMCComparisonCode::CARRY:             return  get_flag(ICMCFlag::CARRY);
        case ICMCComparisonCode::NOT_CARRY:         return !get_flag(ICMCFlag::CARRY);
        case ICMCComparisonCode::GREATER:           return  get_flag(ICMCFlag::GREATER);
        case ICMCComparisonCode::LESS:              return  get_flag(ICMCFlag::LESS);
        case ICMCComparisonCode::GREATER_OR_EQUAL:  return !get_flag(ICMCFlag::LESS);
        case ICMCComparisonCode::LESS_OR_EQUAL:     return !get_flag(ICMCFlag::GREATER);
        case ICMCComparisonCode::OVERFLOW:          return  get_flag(ICMCFlag::OVERFLOW);
        case ICMCComparisonCode::NOT_OVERFLOW:      return !get_flag(ICMCFlag::OVERFLOW);
        case ICMCComparisonCode::NEGATIVE:          return  get_flag(ICMCFlag::NEGATIVE);
        case ICMCComparisonCode::DIV_BY_ZERO:       return  get_flag(ICMCFlag::DIV_BY_ZERO);
        default:                                    return false;
    }
    // clang-format on
}

void ICMCProcessor::execute_jmp() {
    auto addr = fetch_next_word();
    if (should_jump()) {
        program_counter = addr;
    }
}

void ICMCProcessor::execute_call() {
    auto addr = fetch_next_word();
    if (should_jump()) {
        memory[stack_pointer] = program_counter - 1;
        program_counter = addr;
        stack_pointer--;
    }
}

void ICMCProcessor::execute_rts() {
    stack_pointer++;
    program_counter = memory[stack_pointer];
    program_counter++;
}

// Instruções de chamada
void ICMCProcessor::execute_push() {
    if (slice(instruction_register, 6, 6) == 0) {
        memory[stack_pointer] = rx();
    } else {
        memory[stack_pointer] = flag_register;
    }
    stack_pointer--;
}

void ICMCProcessor::execute_pop() {
    stack_pointer++;
    if (slice(instruction_register, 6, 6) == 0) {
        rx() = memory[stack_pointer];
    } else {
        flag_register = memory[stack_pointer];
    }
}

// Instruções de controle
void ICMCProcessor::execute_carry() {
    bool carry = slice(instruction_register, 9, 9);
    set_flag(ICMCFlag::CARRY, carry);
}

void ICMCProcessor::execute_halt() {
    halt = true;
}

void ICMCProcessor::execute_noop() {}

void ICMCProcessor::execute_breakp() {}

// Registradores
uint16_t &ICMCProcessor::rx() {
    return registers[slice(instruction_register, 7, 9)];
}

uint16_t &ICMCProcessor::ry() {
    return registers[slice(instruction_register, 4, 6)];
}

uint16_t &ICMCProcessor::rz() {
    return registers[slice(instruction_register, 1, 3)];
}

uint8_t ICMCProcessor::carry() {
    return slice(instruction_register, 0, 0) and get_flag(ICMCFlag::CARRY);
}

// Operações auxiliares
bool ICMCProcessor::get_flag(ICMCFlag flag) const {
    auto shift_ammount = static_cast<uint8_t>(flag);
    return flag_register & (1 << shift_ammount);
}

void ICMCProcessor::set_flag(ICMCFlag flag, bool value) {
    auto shift_ammount = static_cast<uint8_t>(flag);
    uint16_t mask = (1 << shift_ammount);
    if (value) {
        flag_register |= mask;
    } else {
        flag_register &= ~mask;
    }
}

uint16_t ICMCProcessor::fetch_next_word() {
    return memory[program_counter++];
}

ICMCProcessor::ICMCProcessor(uint16_t *memory) : memory(memory) {}

void ICMCProcessor::run() {
    while (!halt) {
        execute_next_instruction();
//        sleep_ms(1);
    }
}

void ICMCProcessor::store_add(uint32_t y, uint32_t z) {
    uint32_t value = y + z;

    if (value > REGISTER_MAX_VALUE) {
        set_flag(ICMCFlag::CARRY, true);
        value = REGISTER_MAX_VALUE;
    }

    store_alu(value);
}

void ICMCProcessor::store_sub(uint32_t y, uint32_t z) {
    int32_t value = (int32_t)y - (int32_t)z;

    if (value < 0) {
        set_flag(ICMCFlag::NEGATIVE, true);
        value = REGISTER_MAX_VALUE + value;
    }

    store_alu(value);
}

void ICMCProcessor::store_alu(uint32_t value) {
    reset_arithmetic_flags(value);
    rx() = value;
}

void ICMCProcessor::reset_arithmetic_flags(uint32_t value) {
    set_flag(ICMCFlag::CARRY, false);
    set_flag(ICMCFlag::NEGATIVE, false);
    set_flag(ICMCFlag::OVERFLOW, false);
    set_flag(ICMCFlag::DIV_BY_ZERO, false);
    set_flag(ICMCFlag::ZERO, value == 0);
}

void ICMCProcessor::reset_logic_flags() {
    set_flag(ICMCFlag::EQUAL, false);
    set_flag(ICMCFlag::GREATER, false);
    set_flag(ICMCFlag::LESS, false);
}