CREATE TABLE IF NOT EXISTS Mitra_datatable (ind text, keyword text, values text);
CREATE TABLE IF NOT EXISTS Mitra_indextable (addr text, val text);

-- CREATE TABLE IF NOT EXISTS athletes_data (ind text, name text, short_name text);
-- CREATE TABLE IF NOT EXISTS athletes_index (UT text, en_indop text);

DROP FUNCTION IF EXISTS mitra_insert_bulk(TEXT[], TEXT[], TEXT[], TEXT[], TEXT[]);
CREATE OR REPLACE FUNCTION mitra_insert_bulk(p_ind text[], p_keyword text[], p_value text[], p_addr text[], p_val text[]) RETURNS VOID AS $$
DECLARE
    sql_query1 TEXT;
    sql_query2 TEXT;
BEGIN
    -- 构建插入语句
    sql_query1 := 'INSERT INTO Mitra_datatable (ind, keyword, values) VALUES ';
    sql_query2 := 'INSERT INTO Mitra_indextable (addr, val) VALUES ';
    -- sql_query1 := 'INSERT INTO athletes_data (ind, name, short_name) VALUES ';
    -- sql_query2 := 'INSERT INTO athletes_index (UT, en_indop) VALUES ';
    FOR i IN 1..array_length(p_ind, 1) LOOP
        sql_query1 := sql_query1 || '(' || quote_literal(p_ind[i]) || ', ' || quote_literal(p_keyword[i]) || ', ' || quote_literal(p_value[i]) || '),';
        sql_query2 := sql_query2 || '(' || quote_literal(p_addr[i]) || ', ' || quote_literal(p_val[i]) || '),';
    END LOOP;
    -- 去除最后一个逗号
    sql_query1 := left(sql_query1, length(sql_query1) - 1);
    sql_query2 := left(sql_query2, length(sql_query2) - 1);
    -- 执行插入语句
    EXECUTE sql_query1;
    EXECUTE sql_query2;
END;
$$ LANGUAGE plpgsql;


-- DO $$
-- DECLARE
--     ind_array TEXT[] := ARRAY['1', '2', '3'];
--     keyword_array TEXT[] := ARRAY['add', 'add', 'del'];
--     value_array TEXT[] := ARRAY['value1', 'value2', 'value3'];
-- BEGIN
--     PERFORM fides_insert_bulk(ind_array, keyword_array, value_array);
-- END $$;

-- DROP FUNCTION IF EXISTS fides_insert_bulk(TEXT[], TEXT[], TEXT[]);




DROP FUNCTION IF EXISTS mitra_ind_data(VARCHAR[], VARCHAR[], VARCHAR[]);
CREATE OR REPLACE FUNCTION mitra_ind_data(p_addr VARCHAR[], p_val VARCHAR[], p_ind VARCHAR[])
RETURNS TABLE(ind TEXT, keyword TEXT, context TEXT) AS $$
DECLARE
    insert_sql TEXT;
    search_sql TEXT;
BEGIN
    -- 构建插入数据的动态 SQL 语句
    -- insert_sql := 'INSERT INTO athletes_index (UT, en_indop) VALUES ';
    insert_sql := 'INSERT INTO mitra_indextable (addr, val) VALUES ';
    FOR i IN 1..array_length(p_addr, 1) LOOP
        insert_sql := insert_sql || '(' || quote_literal(p_addr[i]) || ', ' || quote_literal(p_val[i]) || '),';
    END LOOP;
    insert_sql := rtrim(insert_sql, ',');
    
    -- 执行插入操作
    EXECUTE insert_sql;
    
    -- 构建搜索数据的动态 SQL 语句
    -- search_sql := 'SELECT * FROM athletes_data WHERE ind IN (';
    search_sql := 'SELECT * FROM mitra_datatable WHERE ind IN (';
    FOR i IN 1..array_length(p_ind, 1) LOOP
        search_sql := search_sql || quote_literal(p_ind[i]) || ', ';
    END LOOP;
    search_sql := rtrim(search_sql, ', ');--逗号后面有个空格
    search_sql := search_sql || ')';
    -- 执行搜索操作并将结果返回
    RETURN QUERY EXECUTE search_sql;
END;
$$ LANGUAGE plpgsql;

-- 执行函数并获取结果
-- DO $$
-- DECLARE
--     ind_array TEXT[] := ARRAY['1', '2', '3'];
--     ut_array TEXT[] := ARRAY['value1', 'value2', 'value3'];
-- BEGIN
--     PERFORM fides_select(ind_array, ut_array);
-- END $$;
