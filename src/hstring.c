#include "hisp.h"
#include "hstring.h"


/* hp_new_string_n [C API]
 *
 * build new string from c string, must specific string length(exclude '\0')
 * 
 * IMPORTANT:
 *   though begins with `hp_`, this function is never be exposed to Hisp.
 */
HObject hp_new_string_ncptr(const char *str, int length)
{
    /* FIXME: alloc check */
    HString hstr = (HString *)malloc(sizeof(HString));
    hstr->length = length;

    if (length >= HP_MAX_STR_EMBSZ) {
        /* too long to be a embedded string */
        int luxury = length / 2;
        char *buf = (char *)malloc(length + luxury);

        /* never forger the '/0' */
        memcpy(buf, str, length + 1);
        
        hstr->ptr = buf;
        hstr->holds = length + luxury;
    } else {
        /* embedded string can embed to embstr field */
        /* never forger the '/0' */
        memcpy(hstr->embstr, str, length + 1);
    }
    
    return (HObject)hstr;
}

/* hp_new_string_n [C API]
 *
 * build new string from c string, no need to specific string length
 */
HObject hp_new_string_cptr(char *str)
{
    int length = strlen(str);
    return hp_new_string_ncptr(str, length);
}

#define HSTRING_LENGTH(s) (HSTRING(s)->length)

/* hp_string_length
 *
 * counting a string length
 */
HObject hp_string_length(HObject str)
{
    if (HSTRING_P(str))
        return INT2FIX(HSTRING_LENGTH(str));
    else
        return hp_error(RUNTIME_ERROR, "Object str isn's a valid string");
}

HObject hp_symbol_to_string(HObject str)
{
    
}

HObject hp_string_to_symbol(HObject sym)
{
    
}
