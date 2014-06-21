

HObject hp_new_string_n(char *str, int length)
{
    int luxury = length / 2;
    char *buf = (char *)malloc(length + luxury);

    memcpy(buf, str, length);

    HString hstr = (HString *)malloc(sizeof(HString))
    hstr->ptr = buf;
    hstr->holds = length + luxury;
    hstr->length = length - 1;
    
}