#include "helperlib.h"

#define ISSPACE(x) ((x) == ' ' || (x) == '\t' || (x) == '\r' || (x) == '\n')
#define ISNUM(x) ((x) >= '0' && (x) <= '9')

long double atolf(char *s) {
	long double lf = 0.0, temp = 0.0;
	int i = 0, mul = 1, isneg = 0;
	if (!s)
		return lf;

	// Remove leading whitespaces.
	while (ISSPACE(s[i]))
		++i;
	// Consume signs, if any.
	if (s[i] == '+')
		++i;
	else if (s[i] == '-') {
		isneg = 1;
		++i;
	}

	// Consume pre-decimal part.
	for (i = 0; ISNUM(s[i]); ++i) {
		lf *= 10;
		lf += (s[i] - '0');
	}
	// Consume decimal point, if present.
	if (s[i] == '.')
		++i;
	// Consume post-decimal part, if any.
	for (; ISNUM(s[i]); ++i) {
		temp *= 10;
		temp += (s[i] - '0');
		mul *= 10;
	}
	temp /= mul;
	lf += temp;

	return lf;
}

long double pow(long double base, long int exponent) {
	if(exponent == 0)
		return 1;
	if(exponent < 0)
		return (long double)1 / pow(base, -exponent);
	if(exponent % 2 == 0) {
		long double half = pow(base, exponent / 2);
		return half * half;
	}

	return base * pow(base, exponent - 1);
}
