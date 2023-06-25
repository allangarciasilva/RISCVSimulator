#include "stddef.h"
#include "stdint.h"

void *memset(void *str, int c, size_t n) {
    uint8_t *iterator = str;
    while (n--) {
        *iterator++ = (uint8_t)c;
    }
    return str;
}