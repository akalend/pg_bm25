MODULES = pg_bm25

EXTENSION = pg_bm25
DATA = pg_bm25--0.1.sql
PGFILEDESC = "pg_bm25 - module"


ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/pg_bm25
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
