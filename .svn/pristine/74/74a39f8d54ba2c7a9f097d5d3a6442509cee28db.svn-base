/***********************************************************************
 *
 *   This file contains functions for the automatic complexity calculation
 *
*************************************************************************/

#include "typedef.h"
#include "count.h"

/* Global counter variable for calculation of complexity weight */

BASIC_OP counter;

const BASIC_OP op_weight = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 4, 15, 18, 30, 1, 2, 1, 2, 2
};

/* local variable */

#define NbFuncMax  1024

static Word16 funcid;
static Word32 wc[NbFuncMax];
static Word32 LastWOper;

Word32 TotalWeightedOperation() {
	Word16 i;
	Word32 tot, *ptr, *ptr2;
	tot = 0;
	ptr = (Word32 *) &counter;
	ptr2 = (Word32 *) &op_weight;
	for (i = 0; i < (sizeof (counter) / sizeof (Word32)); i++) {
		tot += ((*ptr++) * (*ptr2++));
	}
	return ((Word32) tot);
}

Word32 DeltaWeightedOperation() {
	Word32 NewWOper, delta;
	NewWOper = TotalWeightedOperation ();
	delta = NewWOper - LastWOper;
	LastWOper = NewWOper;
	return (delta);
}

void move16 (void) {
	counter.DataMove16++;
}

void move32 (void) {
	counter.DataMove32++;
}

void test (void) {
	counter.Test++;
}

void logic16 (void) {
	counter.Logic16++;
}

void logic32 (void) {
	counter.Logic32++;
}

Word32 fwc (void) {
	/* function worst case */
	Word32 tot;
	tot = DeltaWeightedOperation ();
	if (tot > wc[funcid]) {
		wc[funcid] = tot;
	}
	funcid++;
	return (tot);
}
