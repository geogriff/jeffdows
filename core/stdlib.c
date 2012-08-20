#include <stdbool.h>
#include <ctype.h>

#include <stdlib.h>

int digit_value(int c, int base) {
  // try numerical digit
  if (isdigit(c)) {
    return c - '0';
  }
  // try alpha digit
  int value = tolower(c) - 'a';
  if (value >= 0 && value < (base - 10)) {
    return value;
  }

  return -1;
}

long int strtol(const char * restrict nptr, char ** restrict endptr,
		int base) {
  // note: this function assumes ascii
  const char * restrict startptr = nptr;

  if (base < 0 || base > 36) {
    goto bad_ret;
  }

  // base 0 means base 10
  base += !base * 10;

  // skip whitespace
  for (;isspace(*nptr); nptr++);
  // parse a sign
  int sign = 1;
  switch (*nptr) {
  case '-':
    sign = -1;
  case '+':
    nptr++;
  }
  // parse a hex prefix
  if (base == 16 && *nptr == '0' && tolower(nptr[1]) == 'x') {
    nptr += 2;
  }
  // parse a number
  int value = 0;
  const char * restrict subjectptr = nptr;
  for (int digit; (digit = digit_value(*nptr, base)) >= 0;
       nptr++) {
    // XXX handle overflow
    value = value * base + digit;
  }
  if (nptr == subjectptr) {
    *endptr = (char *) startptr;
  } else {
    *endptr = (char *) nptr;
  }

  return value * sign;
 bad_ret:
  *endptr = (char *) startptr;
  return 0;
}
