CREATE EXTENSION uuid_decimal;

-- Test functions must return NULL on NULL arg
SELECT uuid_to_decimal(NULL);
SELECT decimal_to_uuid(NULL);

-- Must fail on negative values
SELECT decimal_to_uuid('-1'::decimal);
-- Must fail on fraction values
SELECT decimal_to_uuid('0.1'::decimal);
SELECT decimal_to_uuid('1.1'::decimal);

-- Test UUIDs can be restored to the same value
SELECT decimal_to_uuid(uuid_to_decimal('01923554-9e91-d520-f927-46af2232648c'));
SELECT decimal_to_uuid(uuid_to_decimal('00000000-0000-0000-0000-000000000000'));
SELECT decimal_to_uuid(uuid_to_decimal('ffffffff-ffff-ffff-ffff-ffffffffffff'));
