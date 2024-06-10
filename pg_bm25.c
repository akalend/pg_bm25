/*
 * contrib/debug/debug.c
 */
#include "postgres.h"
#include "fmgr.h"
#include "c.h"

#include <postgres.h>
#include <access/amapi.h>
#include <access/heapam.h>
#include <access/htup_details.h>
#include <access/table.h>
#include <access/tableam.h>
#include <catalog/indexing.h>
#include "utils/builtins.h"
#include "utils/fmgrprotos.h"
#include <utils/fmgroids.h>
#include <utils/rel.h>
#include <utils/snapmgr.h>


PG_MODULE_MAGIC;


PG_FUNCTION_INFO_V1(bm25_test);

/* the tamplate function for debuging */
Datum
bm25_test(PG_FUNCTION_ARGS)
{
	PG_RETURN_TEXT_P(cstring_to_text("test ok"));
}

