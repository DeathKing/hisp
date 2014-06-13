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
	return p == Qnil
}

#define is_empty_list(v) (NIL_P(v))

int list_length(HObject l)
{
	int c = 0;

	while (!NIL_P(l)) {
		c++;
		l = cdr(l);
	}

	return c;
}

HObject list_ref(HObject l, int i)
{
	HObject temp = Qnil;

	if (i >= list_length(l)) {
		hp_error(RUNTIME_ERROR, "Overflowed!");
		return Qundef;
	}

	while (i-- && l) {
		temp = car(l);
		l = cdr(l);
	}

	return temp;
}

#define L0(v) list_ref((v), 0)
#define L1(v) list_ref((v), 1)
#define L2(v) list_ref((v), 2)
#define L3(v) list_ref((v), 3)
#define L4(v) list_ref((v), 4)

int tagged_list(HObject l, int tflag)
{
	if (is_pair(l))
		return tflag == car(l);
	else
		return Qfalse;
}

int tagged_list_id(HObject l, char *id)
{
	if (is_pair(l) && is_id(car(l)))
		return id_is(car(l), id);
	else
		return Qfalse;
}

int is_application(HObject exp)
{
	is_pair(exp);
}

HObject exp_operator(HObject exp)
{
	return car(exp);
}

HObject exp_operands(HObject exp)
{
	return cdr(exp);
}

HObject is_exp_no_operands(HObject exp)
{
	return is_empty_list(exp_operands(exp));
}

HObject require_args(HObject exp, int count)
{
	if (list_length(exp) >= count)
		return Qtrue;
	else {
		hp_error(RUNTIME_ERROR, "Argument too few.");
		return Qfalse;
	}
}