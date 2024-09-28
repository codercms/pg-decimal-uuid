#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/uuid.h"
#include "utils/numeric.h"
#include "utils/memutils.h"
#include <stdint.h>
#include "uint_utils.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(uuid_to_decimal);
PG_FUNCTION_INFO_V1(decimal_to_uuid);

void _PG_init(void);

Datum uuid_to_decimal(PG_FUNCTION_ARGS);
Datum decimal_to_uuid(PG_FUNCTION_ARGS);

// 2^32 = 4294967296
static Numeric numeric_2_pow_32 = NULL;
// 2^64 = 18446744073709551616
static Numeric numeric_2_pow_64 = NULL;

void _PG_init(void)
{
    bool have_error = false;
    MemoryContext oldContext = MemoryContextSwitchTo(TopMemoryContext);

    if (numeric_2_pow_32 == NULL)
    {
        numeric_2_pow_32 = DatumGetNumeric(DirectFunctionCall1(int8_numeric, Int64GetDatum(4294967296)));
    }

    if (numeric_2_pow_64 == NULL)
    {
        numeric_2_pow_64 = numeric_mul_opt_error(numeric_2_pow_32, numeric_2_pow_32, &have_error);
    }

    MemoryContextSwitchTo(oldContext);
}

// Function to convert UUID to decimal
Datum uuid_to_decimal(PG_FUNCTION_ARGS)
{
    bool have_error = false;
    Numeric result;
    Numeric low;

    pg_uuid_t *uuid = PG_GETARG_UUID_P(0);
    uint8_t *bytes = (uint8_t *) uuid->data;

    // Extract four 32-bit parts from the UUID bytes
    uint32_t part1 = Uint32BE(bytes);
    uint32_t part2 = Uint32BE(bytes + 4);
    uint32_t part3 = Uint32BE(bytes + 8);
    uint32_t part4 = Uint32BE(bytes + 12);

    // Create variables to track errors
    // elog(INFO, "Mul Value 2^32: %s, Mul Value: 2^64: %s",
    //      DatumGetCString(DirectFunctionCall1(numeric_out, NumericGetDatum(numeric_2_pow_32))),
    //      DatumGetCString(DirectFunctionCall1(numeric_out, NumericGetDatum(numeric_2_pow_64)))
    // );

    result = int64_to_numeric(part1);
    result = numeric_mul_opt_error(result, numeric_2_pow_32, &have_error);
    result = numeric_add_opt_error(result, int64_to_numeric(part2), &have_error);
    result = numeric_mul_opt_error(result, numeric_2_pow_64, &have_error);

    if (have_error) {
        elog(ERROR, "Numeric ops error with high part");
    }

    low = int64_to_numeric(part3);
    low = numeric_mul_opt_error(low, numeric_2_pow_32, &have_error);
    low = numeric_add_opt_error(low, int64_to_numeric(part4), &have_error);


    if (have_error) {
        elog(ERROR, "Numeric ops error with low part");
    }

    result = numeric_add_opt_error(result, low, &have_error);


    if (have_error) {
        elog(ERROR, "Failed to sum high part with low part");
    }

    // Debug logging to verify result
    // elog(INFO, "Part1: %u, Part2: %u, Part3: %u, Part4: %u, Result Numeric Value: %s",
    //      part1, part2, part3, part4,
    //      DatumGetCString(DirectFunctionCall1(numeric_out, NumericGetDatum(result)))
    // );

    PG_RETURN_NUMERIC(result);
}

// Function to convert decimal back to UUID
Datum decimal_to_uuid(PG_FUNCTION_ARGS)
{
    uint128 numeric = 0;
    uint64_t low, high = 0;
    Numeric decimal_value;
    pg_uuid_t* uuid_res;
    char* numeric_text;

    decimal_value = PG_GETARG_NUMERIC(0);
    uuid_res = (pg_uuid_t*)palloc(sizeof(pg_uuid_t));

    // Scan PostgreSQL numeric into uint128_t
    numeric_text = DatumGetCString(DirectFunctionCall1(numeric_out, NumericGetDatum(decimal_value)));

    if (parse_uint128(numeric_text, &numeric) != 0)
    {
        pfree(numeric_text);
        elog(ERROR, "Cannot cast numeric to uint128");
    }

    // elog(INFO, "Text %s", numeric_text);

    pfree(numeric_text);

    // Split uint128 into two uint64s for UUID
    // Extract high and low parts from uint128
    low  = Uint64LE((uint8_t*) &numeric);
    high = Uint64LE(((uint8_t*)&numeric) + 8);

    // elog(INFO, "low %llu, high %llu", low, high);

    PutUint64BE((uint8_t*)uuid_res->data,     high);
    PutUint64BE((uint8_t*)uuid_res->data + 8, low);

    PG_RETURN_UUID_P(uuid_res);
}
