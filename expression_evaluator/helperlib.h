#ifndef HELPERLIB_INC_GUARD
#define HELPERLIB_INC_GUARD

typedef char * String;

long double atolf(char *s);
long double pow(long double base, long int exponent);

void mktmp();
int tmpputs(String s);
int tmpclose();

#endif