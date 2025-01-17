--
-- VACUUM
--

CREATE TABLE vactst (i INT);
INSERT INTO vactst VALUES (1);
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst VALUES (0);
SELECT count(*) FROM vactst;
DELETE FROM vactst WHERE i != 0;
SELECT * FROM vactst;
VACUUM FULL vactst;
UPDATE vactst SET i = i + 1;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst SELECT * FROM vactst;
INSERT INTO vactst VALUES (0);
SELECT count(*) FROM vactst;
DELETE FROM vactst WHERE i != 0;
VACUUM (FULL) vactst;
DELETE FROM vactst;
SELECT * FROM vactst;

VACUUM (FULL, FREEZE) vactst;
VACUUM (ANALYZE, FULL) vactst;

CREATE TABLE vaccluster (i INT PRIMARY KEY);
ALTER TABLE vaccluster CLUSTER ON vaccluster_pkey;
CLUSTER vaccluster;

CREATE FUNCTION do_analyze() RETURNS VOID VOLATILE LANGUAGE SQL
	AS 'ANALYZE pg_am';
CREATE FUNCTION wrap_do_analyze(c INT) RETURNS INT IMMUTABLE LANGUAGE SQL
	AS 'SELECT $1 FROM do_analyze()';
CREATE INDEX ON vaccluster(wrap_do_analyze(i));
INSERT INTO vaccluster VALUES (1), (2);
ANALYZE vaccluster;

set xc_maintenance_mode = on;
VACUUM FULL pg_am;
VACUUM FULL pg_class;
VACUUM FULL pg_database;
set xc_maintenance_mode = off;
VACUUM FULL vaccluster;
VACUUM FULL vactst;

-- check behavior with duplicate column mentions
VACUUM ANALYZE vaccluster(i,i);
ANALYZE vaccluster(i,i);

create table vacuum_p_a(a int) partition by range(a)(partition a1 values less than(100), partition a2 values less than(maxvalue));
insert into vacuum_p_a select generate_series(1,1000);
select relname,last_vacuum,last_autovacuum,vacuum_count,autovacuum_count
from pg_stat_user_tables
where relid in (select oid from pg_class where relname like 'vacuum_p_a');
vacuum vacuum_p_a;
select pg_sleep(1);
select relname,vacuum_count,autovacuum_count,last_vacuum,last_autovacuum
from pg_stat_user_tables
where relid in (select oid from pg_class where relname like 'vacuum_p_a');


DROP TABLE vaccluster;
DROP TABLE vactst;
DROP TABLE vacuum_p_a;
