#include <ctype.h>

int isdigit(int c) {
  return c >= '0' && c <= '9';
}

/* the below functions all assume ascii for now */

int isupper(int c) {
  return c >= 'A' && c <= 'Z';
}
int islower(int c) {
  return c >= 'a' && c <= 'z';
}
int isspace(int c) {
  return c > 0 && c <= ' ';
}

int toupper(int c) {
  if (islower(c)) {
    return c - ('a' - 'A');
  }
  return c;
}
int tolower(int c) {
  if (isupper(c)) {
    return c - ('A' - 'a');
  }
  return c;
}
