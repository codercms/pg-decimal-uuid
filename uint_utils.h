#include "postgres.h"
#include <stdint.h>

uint32_t Uint32LE(uint8_t*);
uint32_t Uint32BE(uint8_t*);
uint64_t Uint64LE(uint8_t*);
uint64_t Uint64BE(uint8_t*);

void PutUint64BE(uint8_t* b, uint64_t v);

int parse_uint128(const char* str, uint128* result);
