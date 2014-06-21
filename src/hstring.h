#ifndef _HISP_STRING_
#define _HISP_STRING_

typedef struct hp_char   HChar;
typedef struct hp_string HString;

struct hp_char {
    HBasic basic;
    char ch;
};

struct hp_string {
    HBasic basic;
    int length;
    int holds;
    char *ptr;
};

#endif