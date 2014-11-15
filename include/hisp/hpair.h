#ifndef _HISP_PAIR_
#define _HISP_PAIR_

struct hp_pair {
	HBasic  basic;
	HObject car;
	HObject cdr;	
};

typedef struct hp_pair HPair;

#define HPAIR(v) ((HPair *)(v))
#define HEMPTYLIST Qnull

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

#endif
