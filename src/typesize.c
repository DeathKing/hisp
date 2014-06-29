#include <stdio.h>

#define S(t) sizeof(t)
#define log(t) printf("%20s = %d\n", " # t # ", S(t));

int main()
{
    printf("%20s = %lu\n", "short int", S(short int));
    printf("%20s = %lu\n", "int", S(int));
    printf("%20s = %lu\n", "long", S(long));
    printf("%20s = %lu\n", "long long", S(long long));
    printf("%20s = %lu\n", "void *", S(void *));
    printf("%20s = %lu\n", "char", S(char));
    printf("%20s = %lu\n", "signed char", S(signed char));

    return 0;
}