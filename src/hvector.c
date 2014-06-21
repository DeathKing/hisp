HObject hp_new_vector_n(int k)
{

    HObject *buf = (HObject *)malloc(sizeof(HObject) * k);
    HVector  vec = (HVector *)malloc(sizeof(HVector));

    HVECTOR(vec)->head   = buf;
    HVECTOR(vec)->length = k;
    HVECTOR(vec)->basic.type |= TVector;

    return vec;
}
