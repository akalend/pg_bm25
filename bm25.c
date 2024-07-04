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

#define TABLE_NAME "pg_bm25_words"
#define NAME_IDX_NAME "pg_bm25_words_pkey"


typedef struct FormData_pg_bm25_wors
{
	Name word; 		// word   | text
	int32 doc_id;	// doc_id | integer
	int32 id;		// id     | integer
} FormData_pg_bm25_wors;

typedef FormData_pg_bm25_wors* Form_pg_bm25_wors;


typedef enum Anum_pg_bm25_wors
{
	Anum_pg_bm25_words_word = 1,
	Anum_pg_bm25_words_docid,
	Anum_pg_bm25_wordid,
	Natts_pg_bm25_words,
} Anum_pg_bm25_wors;


static Oid
name_to_oid(const char* name)
{
	return DatumGetObjectId(DirectFunctionCall1(to_regclass, CStringGetTextDatum(name)));
}





/* the tamplate function for debuging */
Datum
bm25_test(PG_FUNCTION_ARGS)
{
	Relation rel, idxrel;
	IndexScanDesc scan;
	TupleTableSlot* slot;
	HeapTuple tup;
	ScanKeyData skey[2];
	int32 found_doc_id = -1;
	int32 id = 2;
	// text *name =  cstring_to_text("aaaa"); 			//text_to_cstring(PG_GETARG_TEXT_P(0));
	NameData  name;
	Oid tbl_oid = name_to_oid(TABLE_NAME);
	Oid idx_oid = name_to_oid(NAME_IDX_NAME);

	memset(name.data, '\0', NAMEDATALEN);
	strcpy( name.data, "aaaa" );
	

	elog(WARNING, "table oid=%d", tbl_oid);
	elog(WARNING, "index oid=%d", idx_oid);

	rel = table_open(tbl_oid, AccessShareLock);
	idxrel = index_open(idx_oid, AccessShareLock);
	
	scan = index_beginscan(rel, idxrel, 
				GetTransactionSnapshot(), 
				1 /* nkeys */, 
				0 /* norderbys */);

	ScanKeyInit(&skey[0],
				Anum_pg_bm25_words_word,  /* numeration starts from 1; idx, not rel! */
				BTEqualStrategyNumber, F_NAMEEQ,
				NameGetDatum(&name));
	ScanKeyInit(&skey[1],
				Anum_pg_bm25_words_docid,
				BTEqualStrategyNumber, F_INT4EQ,
				Int32GetDatum(id));
	index_rescan(scan, skey, 
				1,    	/* key count */
				NULL 	/* orderbys */,
				 0 		/* norderbys */);


	slot = table_slot_create(rel, NULL);
	while (index_getnext_slot(scan, ForwardScanDirection, slot))
	{
		Form_pg_bm25_wors record;
		bool should_free;

		tup = ExecFetchSlotHeapTuple(slot, false, &should_free);
		if (HeapTupleIsValid(tup))
		{
			Name tmp;
			record = (Form_pg_bm25_wors) GETSTRUCT(tup);
			tmp = record->word;

			elog(WARNING,"is tuple free=%d ", should_free);
			if(strcmp( tmp->data, name.data) == 0)
			{
				found_doc_id = record->id;
				if(should_free) heap_freetuple(tup);
				break;
			}
		}
		else
		{
			elog(WARNING, "HeapTuple Is not Valid");
		}

		if(should_free) heap_freetuple(tup);
	}

	index_endscan(scan);
	ExecDropSingleTupleTableSlot(slot);
	table_close(idxrel, AccessShareLock);
	table_close(rel, AccessShareLock);
	PG_RETURN_INT32(found_doc_id);
}