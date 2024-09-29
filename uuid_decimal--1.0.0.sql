-- uuid_decimal--1.0.0.sql

CREATE OR REPLACE FUNCTION uuid_to_decimal(uuid)
RETURNS numeric
AS 'uuid_decimal', 'uuid_to_decimal'
LANGUAGE C STRICT IMMUTABLE PARALLEL SAFE;

CREATE OR REPLACE FUNCTION decimal_to_uuid(numeric)
RETURNS uuid
AS 'uuid_decimal', 'decimal_to_uuid'
LANGUAGE C STRICT IMMUTABLE PARALLEL SAFE;
