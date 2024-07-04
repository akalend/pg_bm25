CREATE OR REPLACE FUNCTION bm25_test()
RETURNS int
AS 'MODULE_PATHNAME', 'bm25_test'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE TABLE IF NOT EXISTS pg_bm25_words( word text NOT NULL, doc_id int, id integer, PRIMARY KEY(word,doc_id));
-- CREATE INDEX IF NOT EXISTS pg_bm25_words_idx_id ON pg_bm25_words (id);
-- CREATE INDEX IF NOT EXISTS pg_bm25_words_hidx_id ON pg_bm25_words  USING HASH(id); -- ???


