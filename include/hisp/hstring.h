#ifndef _HISP_STRING_
#define _HISP_STRING_

typedef struct hp_char   HChar;
typedef struct hp_string HString;

struct hp_char {
    HBasic basic;
    char ch;
};


#define HP_MAX_STR_EMBSZ (sizeof(char *) + sizeof(int))

struct hp_string {
    HBasic basic;
    int length;
    union {
        char embstr[HP_MAX_STR_EMBSZ];
        union {
            int holds;
            char *ptr;
        };
    }；
};

#endif