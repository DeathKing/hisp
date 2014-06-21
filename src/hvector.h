#ifndef _HISP_VECTOR_
#define _HISP_VECOTR_

typedef hp_vector HVector;

struct hp_vector {
    HBasic basic;
    int length;
    HObject *head;
};

#endif