HObject hp_new_vector(long k)
{

    HObject *buf = (HObject *)malloc(sizeof(HObject) * k);
    HVector *vec = (HVector *)malloc(sizeof(HVector));

    vec->head   = buf;
    vec->length = k;

    HSETFLAGS(vec->basic, TVector)

    return (HObject)vec;
}

HObject hp_new_vector2(long k, HObject fill)
{
    long i = 0;
    HObject vec = hp_new_vector(k);

    for (i = 0; i < k; i++)
        HVECTOR(vec)->head[k] = fill;

    return vec;
}


