#include "uint_utils.h"

uint32_t Uint32BE(uint8_t* b) {
        return (uint32_t)(b[3]) | (uint32_t)(b[2])<<8 | (uint32_t)(b[1])<<16 | (uint32_t)(b[0])<<24;
}

uint32_t Uint32LE(uint8_t* b) {
        return (uint32_t)(b[0]) | (uint32_t)(b[1])<<8 | (uint32_t)(b[2])<<16 | (uint32_t)(b[3])<<24;
}

uint64_t Uint64BE(uint8_t* b) {
        return (uint64_t)(b[7]) | (uint64_t)(b[6])<<8 | (uint64_t)(b[5])<<16 | (uint64_t)(b[4])<<24 |
                (uint64_t)(b[3])<<32 | (uint64_t)(b[2])<<40 | (uint64_t)(b[1])<<48 | (uint64_t)(b[0])<<56;
}

uint64_t Uint64LE(uint8_t* b) {
        return (uint64_t)(b[0]) | (uint64_t)(b[1])<<8 | (uint64_t)(b[2])<<16 | (uint64_t)(b[3])<<24 |
                (uint64_t)(b[4])<<32 | (uint64_t)(b[5])<<40 | (uint64_t)(b[6])<<48 | (uint64_t)(b[7])<<56;
}

uint128 Uint128BE(uint8_t* b) {
        return (uint128)(b[15]) | (uint128)(b[14])<<8 | (uint128)(b[13])<<16 | (uint128)(b[12])<<24 |
                (uint128)(b[11])<<32 | (uint128)(b[10])<<40 | (uint128)(b[9])<<48 | (uint128)(b[8])<<56 |
                (uint128)(b[7])<<64 | (uint128)(b[6])<<72 | (uint128)(b[5])<<80 | (uint128)(b[4])<<88 |
                (uint128)(b[3])<<96 | (uint128)(b[2])<<104 | (uint128)(b[1])<<112 | (uint128)(b[0])<<120;
}

void PutUint64BE(uint8_t* b, uint64_t v) {
        b[0] = (uint8_t)(v >> 56);
        b[1] = (uint8_t)(v >> 48);
        b[2] = (uint8_t)(v >> 40);
        b[3] = (uint8_t)(v >> 32);
        b[4] = (uint8_t)(v >> 24);
        b[5] = (uint8_t)(v >> 16);
        b[6] = (uint8_t)(v >> 8);
        b[7] = (uint8_t)(v);
}

// Function to parse uint128 from string
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

// Function to convert uint128_t to a decimal string
// The caller must provide a buffer of sufficient size
char* uint128_to_string(uint128 value, char* buffer, size_t buffer_size) {
    char* ptr;

    if (buffer_size < 40) {
        return NULL; // Not enough space
    }

    ptr = buffer + buffer_size - 1; // Start from the end of the buffer
    *ptr = '\0'; // Null-terminate the string

    // Handle zero case explicitly
    if (value == 0) {
        strcpy(buffer, "0");
        return buffer;
    }

    while (value > 0) {
        // Get the last digit
        uint64_t digit = value % 10;
        value /= 10;
        // Prepend the digit to the string
        *(--ptr) = '0' + digit;
    }

    return ptr; // Return the pointer to the start of the string
}
