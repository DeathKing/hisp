
/* hp_new_string_n [C API]
 *
 * IMPORTANT:
 *   though begins with `hp_`, this function is never be exposed to Hisp.
 */
HObject hp_new_string_n(char *str, int length)
{
    int luxury = length / 2;
    char *buf = (char *)malloc(length + luxury);

    memcpy(buf, str, length);

    /* FIXME: alloc check */
    HString hstr = (HString *)malloc(sizeof(HString));

    hstr->ptr = buf;
    hstr->holds = length + luxury;
    hstr->length = length - 1;
    
    return (HObject)hstr;
}

/* hp_string_length
 *
 * counting a string length
 */
HObject hp_string_length(HObject str)
{
    if (HSTRING_P(str))
        return INT2FIX(HSTRING(str)->length);
    else
        return hp_error(RUNTIME_ERROR, "Object str isn's a valid string");
}

HObject hp_symbol_to_string(HObject str)
{
    
}

HObject hp_string_to_symbol(HObject sym)
{
    
}