#include "uint_utils.h"

uint32_t Uint32BE(uint8_t* b) {
        return (uint32_t)(b[3]) | (uint32_t)(b[2])<<8 | (uint32_t)(b[1])<<16 | (uint32_t)(b[0])<<24;
}

uint32_t Uint32LE(uint8_t* b) {
        return (uint32_t)(b[0]) | (uint32_t)(b[1])<<8 | (uint32_t)(b[2])<<16 | (uint32_t)(b[3])<<24;
}

uint64_t Uint64BE(uint8_t *b) {
        return (uint64_t)(b[7]) | (uint64_t)(b[6])<<8 | (uint64_t)(b[5])<<16 | (uint64_t)(b[4])<<24 |
                (uint64_t)(b[3])<<32 | (uint64_t)(b[2])<<40 | (uint64_t)(b[1])<<48 | (uint64_t)(b[0])<<56;
}

uint64_t Uint64LE(uint8_t *b) {
        return (uint64_t)(b[0]) | (uint64_t)(b[1])<<8 | (uint64_t)(b[2])<<16 | (uint64_t)(b[3])<<24 |
                (uint64_t)(b[4])<<32 | (uint64_t)(b[5])<<40 | (uint64_t)(b[6])<<48 | (uint64_t)(b[7])<<56;
}

void PutUint64BE(uint8_t *b, uint64_t v) {
        b[0] = (uint8_t)(v >> 56);
        b[1] = (uint8_t)(v >> 48);
        b[2] = (uint8_t)(v >> 40);
        b[3] = (uint8_t)(v >> 32);
        b[4] = (uint8_t)(v >> 24);
        b[5] = (uint8_t)(v >> 16);
        b[6] = (uint8_t)(v >> 8);
        b[7] = (uint8_t)(v);
}

int parse_uint128(const char* str, uint128* result) {
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return -1;
        }

        // Shift left by 10 and add the new digit
        *result = *result * 10 + (str[i] - '0');
    }

    return 0;
}
