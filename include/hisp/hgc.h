#ifndef _HISP_GC_
#define _HISP_GC_

#define HP_PAIR_SLOT_SIZE   65535L

struct hp_pair_slot {
    HPair *slot;
    char *bitmap;
    long long last_free;
    unsigned long long size;
    unsigned long long remain;
};

#endif