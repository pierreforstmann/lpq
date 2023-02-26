CREATE SCHEMA test1;
SET search_path = test1;
CREATE TABLE test1 (i int4, t text);
INSERT INTO test1 values (1, 'Hello');
INSERT INTO test1 values (2, 'Goodbye');
