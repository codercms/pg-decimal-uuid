# pg-uuid-decimal
PostgreSQL extension that allows to convert UUID to 128-bit decimal and vice-versa

## Build
### Linux
* Add PostgreSQL repository to your package manager, e.g. see this for Ubuntu https://www.postgresql.org/download/linux/ubuntu/
* Install PostgreSQL version you want, e.g. `sudo apt-get install postgresql-15`
* Install PostgreSQL dev headers, e.g. `sudo apt-get install postgresql-server-dev-15`
* Install build tools `sudo apt-get install build-essentials`
* Clone repository `git clone https://github.com/codercms/pg-uuid-decimal.git && cd pg-uuid-decimal`
* Compile extension `make`
* Copy extension files to pg catalogs `sudo make install`

### Windows
* Install `MSYS2` from https://www.msys2.org/
* Launch MINGW64 console and execute following commands:
  * `pacman -U https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-postgresql-15.3-3-any.pkg.tar.zst` (use your PostgreSQL version here)
  * `pacman --needed -S git mingw-w64-x86_64-gcc base-devel`
  * `git clone https://github.com/codercms/pg-uuid-decimal.git && cd pg-uuid-decimal`

Then run `make` and copy those files to `POSTGRES_PATH\share\extension` (e.g. `C:\Program Files\PostgreSQL\15\share\extension`):
* `uuid_decimal.control`
* `uuid_decimal*.sql`

And copy `uuid_decimal.dll` to `POSTGRES_PATH\lib` (e.g. `C:\Program Files\PostgreSQL\15\lib`)

## Testing
**Make sure you run tests on the build environment database, not on the production one.**

* Create PostgreSQL user to run tests `CREATE USER uuid_decimal_test WITH PASSWORD 'test-suite';`
* Grant superuser role to newly created user `ALTER ROLE uuid_decimal_test SUPERUSER;`
* Run `make PGHOST=127.0.0.1 PGUSER=uuid_decimal_test PGPASSWORD=test-suite installcheck`

## Installation
Open psql console from postgres superuser and run on database you want - `CREATE EXTENSION uuid_decimal;`

## Functions
* `uuid_to_decimal` - Converts UUID to 128-bit decimal representation
* `decimal_to_uuid` - Converts decimal back to UUID

## Performance
CPU: 13th Gen Intel(R) Core(TM) i7-13700HX

Here is the results on PostgreSQL 15 (Ubuntu 24.04 LTS) for 100 000 iterations:

```
postgres=# create extension uuid_decimal;
CREATE EXTENSION
postgres=# CREATE TEMPORARY TABLE uuids AS
SELECT gen_random_uuid() AS id FROM generate_series(1, 100000);
SELECT 100000
postgres=# EXPLAIN ANALYZE VERBOSE
SELECT decimal_to_uuid(uuid_to_decimal(id))
FROM uuids;
                                                      QUERY PLAN
----------------------------------------------------------------------------------------------------------------------
 Seq Scan on pg_temp.uuids  (cost=0.00..2042.27 rows=100085 width=16) (actual time=0.014..35.706 rows=100000 loops=1)
   Output: decimal_to_uuid(uuid_to_decimal(id))
 Planning Time: 0.135 ms
 Execution Time: 37.781 ms
(4 rows)
```
