PG_CONFIG   ?= pg_config
PGLIBDIR 	 = $(shell $(PG_CONFIG) --libdir)
PGINCLUDEDIR = $(shell $(PG_CONFIG) --includedir-server)
CC			 = gcc
LIBS		 = -lm
MODULES		 = fn_compute_determinant

PGXS = $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
