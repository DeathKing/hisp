#ifndef _HISP_PAIR_
#define _HISP_PAIR_

struct hp_pair {
	HBasic  basic;
	HObject car;
	HObject cdr;	
};

typedef struct hp_pair HPair;

#define HPAIR(v) ((HPair *)(v))

#endif
