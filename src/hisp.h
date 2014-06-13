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

/* FIXME: HObject must be a machine word and platform free. */
typedef unsigned long HObject;

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

#define TYPE_MASK 0x00ff
#define TYPE(v) ((HASIC(v)->type) & TYPE_MASK)

/* HISP type system */
enum hp_type {
	TFixnum = 0x01
};
typedef enum hp_type HType;

#define c2h_number(v) ((v)<<1 | TFixnum)
#define h2c_number(v) ((v)>>1)

#define COMPOUND_P(v) ((HOBJECT(v) & TFixnum) != 1)
#define     NULL_P(v) (HOBJECT(v) == Qnull)
#define     TRUE_P(v) (HOBJECT(v) == Qtrue)
#define    FALSE_P(v) (HOBJECT(v) == Qfalse)
#define    UNDEF_P(v) (HOBJECT(v) == Qundef)
#define      EOF_P(v) (HOBJECT(v) == Qeof)
#define     BOOL_P(v) (NIL_P(v) || TRUE_P(v) || FALSE_P(v))

/* bool test in C layer */
#define HTEST(v) (((HObject)(v) & ~Qnull) != 0)

#endif
