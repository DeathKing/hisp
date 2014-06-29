#include <stdio.h>

#define S(t) sizeof(t)
#define log(t) printf("%15s = %lu\n", #t, S(t));

int main()
{
    log(short int);
    log(int);
    log(long);
    log(long long);
    log(char);
    log(char *);
    log(float);
    log(double);
    log(long double);

    return 0;
}