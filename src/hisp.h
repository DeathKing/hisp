#ifndef _HISP_
#define _HISP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Constant Definition
*/

#define HFALSE  0
#define HTRUE   !HTRUE

#define HISP_VERSION "0.0.1"
#define HISP_DEV     HTRUE

#if sizeof(unsigned long) == sizeof(void *)
/* FIXME: HObject must be a machine word and platform free. */
typedef unsigned long HObject;
#elif sizeof(unsigned long long) == sizeof(void *)
typedef unsigned long long HObject;
#else
#error "Cannot create basic HObject."
#endif

struct hp_basic {
	unsigned long type;
};
typedef struct hp_basic HBasic;


#define Qfalse ((HObject)0)
#define Qtrue  ((HObject)2)
#define Qnull  ((HObject)4)
#define Qundef ((HObject)6)
#define Qeof   ((HObject)8)

#define HBASIC(v)  ((HBasic *)(v)) 
#define HOBJECT(v) ((HObject)(v))

#define TYPE_MASK 0x00f
#define TYPE(v) ((HBASIC(v)->type) & TYPE_MASK)

/* HISP type system */
enum hp_type {
	TFixnum = 0x01
};
typedef enum hp_type HType;

#define c2h_number(v) ((v)<<1 | TFixnum)
#define h2c_number(v) ((v)>>1)

#define HCOMPOUND_P(v) ((HOBJECT(v) & TFixnum) != 1)
#define     HNULL_P(v) (HOBJECT(v) == Qnull)
#define     HTRUE_P(v) (HOBJECT(v) == Qtrue)
#define    HFALSE_P(v) (HOBJECT(v) == Qfalse)
#define    HUNDEF_P(v) (HOBJECT(v) == Qundef)
#define      HEOF_P(v) (HOBJECT(v) == Qeof)
#define     HBOOL_P(v) (HNIL_P(v) || HTRUE_P(v) || HFALSE_P(v))

/* bool test in C layer */
#define HTEST(v) (((HObject)(v) & ~Qnull) != 0)


#endif
