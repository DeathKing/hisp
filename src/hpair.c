#include "hisp.h"
#include "hpair.h"
#include "herror.h"

#define HPAIR_P(p) (COMPOUND_P(p) && (TYPE(p) & T_PAIR))

HObject hp_cons(HObject x, HObject y)
{
	HPair *p = (HPair *)malloc(sizeof(HPair));

	/* FIXME: alloc check */
	p->basic.type = TPair;
	p->car = x;
	p->cdr = y;

	return (HObject)p; 
}

HObject hp_new_null_pair()
{
	return hp_cons(Qnull, Qnull);
}

HObject hp_car(HObject p)
{
	if (HPAIR_P(p))
		return HPAIR(p)->car;
	else
		return hp_error(RUNTIME_ERROR, "HObject isn't a pair!");
}

HObject hp_cdr(HObject p)
{
	if (HPAIR_P(p))
		return HPAIR(p)->cdr;
	else
		return hp_error(RUNTIME_ERROR, "HObject isn't a pair!");
}

#define caar(p) hp_car(hp_car(p))
#define cadr(p) hp_car(hp_cdr(p))
#define cdar(p) hp_cdr(hp_car(p))
#define cddr(p) hp_cdr(hp_cdr(p))

#define caaar(p) hp_car(hp_car(hp_car(p)))
#define caadr(p) hp_car(hp_car(hp_cdr(p)))
#define cadar(p) hp_car(hp_cdr(hp_car(p)))
#define caddr(p) hp_car(hp_cdr(hp_cdr(p)))
#define cdaar(p) hp_cdr(hp_car(hp_car(p)))
#define cdadr(p) hp_cdr(hp_car(hp_cdr(p)))
#define cddar(p) hp_cdr(hp_cdr(hp_car(p)))
#define cdddr(p) hp_cdr(hp_cdr(hp_cdr(p)))

inline HObject hp_set_car(HObject p, HObject v)
{
	if (HPAIR_P(p))
		return HPAIR(p)->car = v;
	else
		return hp_error(RUNTIME_ERROR, "HObject isn't a pair!");
}

inline HObject hp_set_cdr(HObject p, HObject v)
{
	if (HPAIR_P(p))
		return HPAIR(p)->cdr = v;
	else
		return hp_error(RUNTIME_ERROR, "HObject isn't a pair!");
}


/* #define is_application(p) is_pair(p) */

HObject hp_is_pair(HObject p)
{
	return HPAIR_P(v) ? Qtrue : Qfalse;
}

HObject hp_is_null(HObject p)
{
	return p == Qnil ? Qtrue : Qfalse;
}

int list_length(HObject p)
{
	
}