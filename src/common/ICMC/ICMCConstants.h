#pragma once

struct ICMCOpcode {
    // Instruções de manipulação de dados
    static const uint16_t STORE = 0b110001;
    static const uint16_t LOAD = 0b110000;
    static const uint16_t STOREI = 0b111101;
    static const uint16_t LOADI = 0b111100;
    static const uint16_t LOADN = 0b111000;
    static const uint16_t MOV = 0b110011;

    // Instruções aritméticas
    static const uint16_t ADD = 0b100000;
    static const uint16_t SUB = 0b100001;
    static const uint16_t MULT = 0b100010;
    static const uint16_t DIV = 0b100011;
    static const uint16_t INCDEC = 0b100100;
    static const uint16_t MOD = 0b100101;

    // Instruções lógicas
    static const uint16_t AND = 0b010010;
    static const uint16_t OR = 0b010011;
    static const uint16_t XOR = 0b010100;
    static const uint16_t NOT = 0b010101;
    static const uint16_t SHIFT = 0b010000;
    static const uint16_t CMP = 0b010110;

    // Instruções de entrada e saída
    static const uint16_t INCHAR = 0b110101;
    static const uint16_t OUTCHAR = 0b110010;

    // Instruções de salto e chamada
    static const uint16_t JMP = 0b000010;
    static const uint16_t CALL = 0b000011;
    static const uint16_t RTS = 0b000100;

    // Instruções de chamada
    static const uint16_t PUSH = 0b000101;
    static const uint16_t POP = 0b000110;

    // Instruções de controle
    static const uint16_t CARRY = 0b001000;
    static const uint16_t HALT = 0b001111;
    static const uint16_t NOOP = 0b000000;
    static const uint16_t BREAKP = 0b001110;
};

struct ICMCComparisonCode {
    static const uint16_t ALWAYS = 0b0000;
    static const uint16_t EQUAL = 0b0001;
    static const uint16_t NOT_EQUAL = 0b0010;
    static const uint16_t ZERO = 0b0011;
    static const uint16_t NOT_ZERO = 0b0100;
    static const uint16_t CARRY = 0b0101;
    static const uint16_t NOT_CARRY = 0b0110;
    static const uint16_t GREATER = 0b0111;
    static const uint16_t LESS = 0b1000;
    static const uint16_t GREATER_OR_EQUAL = 0b1001;
    static const uint16_t LESS_OR_EQUAL = 0b1010;
    static const uint16_t OVERFLOW = 0b1011;
    static const uint16_t NOT_OVERFLOW = 0b1100;
    static const uint16_t NEGATIVE = 0b1101;
    static const uint16_t DIV_BY_ZERO = 0b1110;
};

enum class ICMCFlag {
    EQUAL,
    GREATER,
    LESS,
    CARRY,
    NEGATIVE,
    OVERFLOW,
    DIV_BY_ZERO,
    ZERO,
};

struct ICMCColors {
    enum {
        ID_WHITE,
        ID_BROWN,
        ID_GREEN,
        ID_OLIVE,
        ID_NAVY,
        ID_PURPLE,
        ID_TEAL,
        ID_SILVER,
        ID_GRAY,
        ID_RED,
        ID_LIME,
        ID_YELLOW,
        ID_BLUE,
        ID_FUCHSIA,
        ID_AQUA,
        ID_BLACK,
    };

    static const uint16_t HEX_BROWN = 0xa22;
    static const uint16_t HEX_GREEN = 0x0f0;
    static const uint16_t HEX_OLIVE = 0x682;
    static const uint16_t HEX_NAVY = 0x228;
    static const uint16_t HEX_PURPLE = 0x827;
    static const uint16_t HEX_TEAL = 0x088;
    static const uint16_t HEX_SILVER = 0xeef;
    static const uint16_t HEX_GRAY = 0xbbb;
    static const uint16_t HEX_RED = 0xf00;
    static const uint16_t HEX_LIME = 0x3c3;
    static const uint16_t HEX_YELLOW = 0xff0;
    static const uint16_t HEX_BLUE = 0x00f;
    static const uint16_t HEX_FUCHSIA = 0xf2a;
    static const uint16_t HEX_AQUA = 0x7d9;
    static const uint16_t HEX_WHITE = 0xfff;
    static const uint16_t HEX_BLACK = 0x000;

    static uint16_t from_character(uint16_t character) {
        uint16_t color_code = (character >> 8) & 0xf;

        // clang-format off
        switch (color_code) {
            case ICMCColors::ID_WHITE:      return ICMCColors::HEX_WHITE;
            case ICMCColors::ID_BROWN:      return ICMCColors::HEX_BROWN;
            case ICMCColors::ID_GREEN:      return ICMCColors::HEX_GREEN;
            case ICMCColors::ID_OLIVE:      return ICMCColors::HEX_OLIVE;
            case ICMCColors::ID_NAVY:       return ICMCColors::HEX_NAVY;
            case ICMCColors::ID_PURPLE:     return ICMCColors::HEX_PURPLE;
            case ICMCColors::ID_TEAL:       return ICMCColors::HEX_TEAL;
            case ICMCColors::ID_SILVER:     return ICMCColors::HEX_SILVER;
            case ICMCColors::ID_GRAY:       return ICMCColors::HEX_GRAY;
            case ICMCColors::ID_RED:        return ICMCColors::HEX_RED;
            case ICMCColors::ID_LIME:       return ICMCColors::HEX_LIME;
            case ICMCColors::ID_YELLOW:     return ICMCColors::HEX_YELLOW;
            case ICMCColors::ID_BLUE:       return ICMCColors::HEX_BLUE;
            case ICMCColors::ID_FUCHSIA:    return ICMCColors::HEX_FUCHSIA;
            case ICMCColors::ID_AQUA:       return ICMCColors::HEX_AQUA;
            case ICMCColors::ID_BLACK:      return ICMCColors::HEX_BLACK;
            default:                        return ICMCColors::HEX_WHITE;
        }
        // clang-format on
    }
};
