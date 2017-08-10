/**
 * \file hanoi.cpp
 * \brief Hanoi Tower algorithm
 */

#define NUM_ELEM 18

int tower(int, int *, int *, int *, int *, int *, int *);

//arr struct that holds each stag
struct arr {
	int a[NUM_ELEM];
	int b[NUM_ELEM];
	int c[NUM_ELEM];
	int topa;
	int topb;
	int topc;
} hanoi;


int uc_main(void) {
	int i;
	for(i=0 ; i<NUM_ELEM ; i++) {
		//Feeds the elements in the arrs:
		//	-1 = no element
		//	0...NUM_ELEM-1 = elements
		hanoi.a[i] = i;
		hanoi.b[i] = -1;
		hanoi.c[i] = -1;
	}
	//topa, topb, topc, mean the top of each arr
	hanoi.topa = NUM_ELEM-1;
	hanoi.topb = -1;
	hanoi.topc = -1;
	//call to do the job
	tower(NUM_ELEM, hanoi.a, hanoi.c, hanoi.b, &(hanoi.topa), &(hanoi.topc), &(hanoi.topb));
}


int tower(int n, int src[], int dest[], int aux[], int *ts, int *td, int *ta) {
	//the tower function passes the arrs along with the top pointers
	if (n == 1) {
		//if one element is there in source arr, then it is moved to the destination arr,
		dest[++(*td)] = src[(*ts)];
		src[*ts] = -1;
		(*ts)--;
		return 0;
	}
	//else the problem is solved by recursive calls
	tower(n-1, src, aux, dest, ts, ta, td);
	tower(1, src, dest, aux, ts, td, ta);
	tower(n-1, aux, dest, src, ta, td, ts);
	return 0;
}

