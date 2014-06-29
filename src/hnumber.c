#include "hisp.h"
#include "number.h"

HObject hp_new_float(double val)
{
    HFLOAT *f = (HFloat *)malloc(sizeof(HFloa));
    f->basic.type = T_HFLOAT;
    f->value = val;

    return (HObject)f;
}

HObject number_plus(HObject x, HObject y)
{
    if (FIXNUM_P(x)) {
        if (FIXNUM_P(y)) {
            signed long o1 = FIX2SLONG(x);
            signed long o2 = FIX2SLONG(y);
            return SLONG2FIX(o1 + o2);
        } else if (FLOAT_P(y)) {
            signed long o1 = FIX2SLONG(x);
            double o2 = float_val(y);
            return new_float(o1 + o2); 
        }
    } else if (FLOAT_P(x)) {
        if (FIXNUM_P(y)) {
            double o1 = float_val(x);
            signed long o2 = FIX2SLONG(y)
            return new_float(o1 + o2);
        } else if (FLOAT_P(y)) {
            double o1 = float_val(x);
            double o2 = float_val(y);
            return new_float(o1 + o2);
        }
    }
}

HObject fixnum_plus(HObject x, HObject y)
{
    signed long o1 = FIX2SLONG(x);
    signed long o2 = FIX2SLONG(y);

    return SLONG2FIX(o1 + o2);
}

HObject float_plus(HObject x, HObject y)
{
    double o1 = float_val(x);
    double o2 = float_val(y);
    return float_new(o1 + o2);
}

HObject hp_exact2inexact(HObject x)
{
    
}

inline double float_val(HObject f)
{
	return HFLOAT(v)->value;
}

#define is_number(v) ((unsigned long)(v) & 0x1)

int is_float(OBJECT exp)
{
	return COMPOUND_DATA(exp) && (TYPE(exp) == T_HFLOAT);
}
