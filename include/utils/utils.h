#ifndef RL_UTILS_H__
#define RL_UTILS_H__

#include<stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t minu8(uint8_t a, uint8_t b) {
    return a < b ? a : b;
}

uint8_t maxu8(uint8_t a, uint8_t b) {
    return a > b ? a : b;
}

uint16_t minu16(uint16_t a, uint16_t b) {
    return a < b ? a : b;
}

uint16_t maxu16(uint16_t a, uint16_t b) {
    return a > b ? a : b;
}

#ifdef __cplusplus
}
#endif

#endif