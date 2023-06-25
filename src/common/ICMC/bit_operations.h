#pragma once

#include <limits.h>
#include <stdint.h>

template <typename T>
struct BitConstants {
    static const uint8_t BIT_WIDTH = sizeof(T) * CHAR_BIT;
    static const T FULL_SET = static_cast<T>(-1);
};

template <typename T>
inline T slice(T value, uint8_t l, uint8_t r = BitConstants<T>::BIT_WIDTH - 1) {
    T mask = BitConstants<T>::FULL_SET;
    if (r < BitConstants<T>::BIT_WIDTH - 1) {
        mask = ~(BitConstants<T>::FULL_SET << (r + 1));
    }
    return (mask & value) >> l;
}

template <typename T>
inline T rotl(T x, uint8_t n) {
    static const T mask = BitConstants<T>::BIT_WIDTH - 1;
    n &= mask;
    return (x << n) | (x >> (-n & mask));
}

template <typename T>
inline T rotr(T x, uint8_t n) {
    static const T mask = BitConstants<T>::BIT_WIDTH - 1;
    n &= mask;
    return (x >> n) | (x << (-n & mask));
}

template <typename T>
inline T shiftl0(T x, uint8_t n) {
    return x << n;
}

template <typename T>
inline T shiftl1(T x, uint8_t n) {
    if (n == BitConstants<T>::BIT_WIDTH) {
        return BitConstants<T>::FULL_SET;
    }
    return (x << n) | ((1 << n) - 1);
}

template <typename T>
inline T shiftr0(T x, uint8_t n) {
    return x >> n;
}

template <typename T>
inline T shiftr1(T x, uint8_t n) {
    if (n == BitConstants<T>::BIT_WIDTH) {
        return BitConstants<T>::FULL_SET;
    }
    return (x << n) | ~(BitConstants<T>::FULL_SET >> n);
}