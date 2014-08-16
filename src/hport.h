#ifndef _HISP_PORT_
#define _HISP_PORT_

#include <stdio.h>

typedef struct hp_port HPort;

struct hp_port {
    HBasic basic;
    FILE  *cptr;  /* real pointer point to a FILE struct */
};

#endif