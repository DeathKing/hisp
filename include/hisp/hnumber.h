#ifndef _HISP_NUMBER_
#define _HISP_NUMBER_

typedef struct hp_float HFloat;

struct hp_float {
	HBasic basic;
	double value;
};


#define HFLOAT(v)   ((HFloat *)(v))

#endif