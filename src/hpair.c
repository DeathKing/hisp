#include "hisp.h"
#include "hpair.h"
#include "herror.h"

#define HPAIR_P(p) (COMPOUND_P(p) && (TYPE(p) & T_PAIR))

/*
 * (cons 1 2)                      ---> (1 . 2)
 * (cons (cons 1 2) 3)             ---> ((1 . 2) . 3)
 * (cons 1 (cons 2 3))             ---> (1 2 . 3)
 * (cons 1 (cons 2 (cons 3 '())))  ---> (1 2 3)
 */
HObject hp_cons(HObject x, HObject y)
{
	/* FIXME: custom malloc() system */
	HPair *p = (HPair *)malloc(sizeof(HPair));

	/* FIXME: alloc check */
	p->basic.type = TPair;
	p->car = x;
	p->cdr = y;

	return (HObject)p;
}

/*
 * (car (cons 1 2))  ---> 1
 * (car '(1 . 2))    ---> 1
 * (car '())         ---> Error
 */
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


#define CAR(p)     (HPAIR(p)->car)
#define CDR(p)     (HPAIR(p)->cdr)
#define CONS(x, y) hp_cons((x), (y))
#define NEW_PAIR() hp_cons(Qnull, Qnull)

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


/*
 * (define a (cons 3 4))  ---> a
 * (set-car! a 5)         ---> Unspecified return value
 * a                      ---> (5 . 4)
 */
inline HObject hp_set_car_bang(HObject p, HObject v)
{
	if (HPAIR_P(p))
		return HPAIR(p)->car = v;
	else
		return hp_error(RUNTIME_ERROR, "HObject isn't a pair!");
}

/*
 * (define a (cons 3 4))  ---> a
 * (set-cdr! a 5)         ---> Unspecified return value
 * a                      ---> (3 . 5)
 */
inline HObject hp_set_cdr_bang(HObject p, HObject v)
{
	if (HPAIR_P(p))
		return HPAIR(p)->cdr = v;
	else
		return hp_error(RUNTIME_ERROR, "HObject isn't a pair!");
}

/*
 * (pair? (cons 1 2)) ---> #t
 * (pair? (list 1 2)) ---> #t
 * (pair? '())        ---> #f
 */
inline HObject hp_pair_p(HObject p)
{
	return HPAIR_P(v) ? Qtrue : Qfalse;
}

/*
 * (null? '()) ---> #t
 * (null? #f)  ---> #f
 */
inline HObject hp_null_p(HObject p)
{
	return p == Qnil ? Qtrue : Qfalse;
}

/* list_length [C API] 
 *
 * Counting length of a list
 */
int list_length(HPair *p)
{
	int c = 1;

	while ((p = p->cdr) != Qnull)
		c++;

	return c;
}