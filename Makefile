MODULES = uuid_decimal
EXTENSION = uuid_decimal
DATA = uuid_decimal--1.0.0.sql

MODULE_big = $(EXTENSION)
OBJS = uuid_decimal.o uint_utils.o

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
