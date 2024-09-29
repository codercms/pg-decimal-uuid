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
    char* str_buf;
    char *num_str;
    uint128 uuid_num;
    pg_uuid_t* uuid;
    Numeric temp;
    Numeric result;

    uuid = PG_GETARG_UUID_P(0);
    uuid_num = Uint128BE((uint8_t*)uuid->data);

    str_buf = palloc(41);
    num_str = uint128_to_string(uuid_num, str_buf, 41);

    temp = DatumGetNumeric(DirectFunctionCall1(
        numeric_in,
        CStringGetDatum(num_str)
    ));

    result = DatumGetNumeric(DirectFunctionCall1(
        numeric_trim_scale,
        NumericGetDatum(temp)
    ));

    pfree(str_buf);
    pfree(temp);

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
