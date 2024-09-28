CREATE EXTENSION uuid_decimal;
SELECT decimal_to_uuid(uuid_to_decimal('01923554-9e91-d520-f927-46af2232648c'));
SELECT decimal_to_uuid(uuid_to_decimal('00000000-0000-0000-0000-000000000000'));
SELECT decimal_to_uuid(uuid_to_decimal('ffffffff-ffff-ffff-ffff-ffffffffffff'));
