#define USE_TYPED_DSET
#include <stdlib.h>
#include <stdio.h>

#include <devSup.h>
#include <longinRecord.h>
#include <aiRecord.h>
#include <epicsVersion.h>
#include <epicsExport.h>

static long longin_init_record(struct dbCommon* rec);
static long longin_read(longinRecord* rec);

#ifndef HAS_longindset
typedef struct {
  dset common;
  long (*read_longin)(longinRecord* record);
} longindset;
#endif

longindset devLongRandom = {
  .common = {
    .number = 5,
    .report = NULL,
    .init = NULL,
    .init_record = longin_init_record,
    .get_ioint_info = NULL,
  },
  .read_longin = longin_read,
};

epicsExportAddress(dset, devLongRandom);

struct longinPvt {
	int rand_max;
	int off;
};

static long longin_init_record(struct dbCommon* rec)
{
	longinRecord* prec = (longinRecord*)rec;
	prec->dpvt = malloc(sizeof(struct longinPvt));
	struct longinPvt* pvt = prec->dpvt;

	if (prec->inp.type == INST_IO) {
		sscanf(prec->inp.value.instio.string, "%d,%d", &pvt->rand_max, &pvt->off);
	}

	return 0;
}

static long longin_read(longinRecord* rec)
{
	longinRecord* prec = rec;
	struct longinPvt* pvt = prec->dpvt;

	prec->val = rand() % pvt->rand_max + pvt->off;

	return 0;
}

