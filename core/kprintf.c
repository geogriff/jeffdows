#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <mem/page.h>
#include <types.h>
#include <ctype.h>
#include <string.h>
#include "kprintf.h"

int kprintf(const char * restrict format, ...) {
  va_list ap;
  va_start(ap, format);
  int chars = 0;
  for (; *format; format++) {
    // parse ordinary characters and "%%"
    if (*format != '%' || *(++format) == '%') {
      putchar(*format);
      chars++;
      continue;
    }

    // parse flags
    bool left = false, plus = false, space = false, alt = false, 
      leading = false;
    for (bool end = false; !end;) {
      switch (*format) {
      case '-': {
	left = true;
      } break;
      case '+': {
	plus = true;
      } break;
      case ' ': {
	space = true;
      } break;
      case '#': {
	alt = true;
      } break;
      case '0': {
	leading = true;
      } break;
      default: {
	end = true;
      }
      }

      if (!end) {
	format++;
      }
    }

    // parse field width
    int field_width = 0;
    if (*format == '*') {
      format++;
      // retrieve field width from next parameter
      
    } else if (isdigit(*format)) {
      // retrieve field width from format string
      field_width = strtol(format, (char**) &format, 10);
    }
    if (field_width < 0) {
      /* A negative field width argument is taken as a '-' flag followed by a
	 positive field width */
      left = true;
      field_width *= -1;
    }

    // parse precision
    int precision = -1;
    if (*format == '.') {
      format++;
      if (*format == '*') {
	// retrieve precision from next parameter
      } else if (isdigit(*format)) {
	// retrieve precision from format string
	precision = strtol(format, (char**) &format, 10);
      } else {
	// if only the period is specified, the precision is taken as zero
	precision = 0;
      }
    }

    // parse length modifier
    int rank = 0;
    char lengthmod = format[0];
    switch (lengthmod) {
    case 'h': {
      rank++;
      if (format[1] == 'h') {
	rank++;
      }
    } break;
    case 'l': {
      rank++;
      if (format[1] == 'l') {
	rank++;
      }
    } break;
    case 'L': {
      rank++;
    } break;
    case 't':
    case 'z':
    case 'j': {
      rank++;
    } break;
    }
    format += rank;

    // parse coversion specifier
    char conv = *format;
    bool upper = isupper(conv);
    if (upper) {
      conv = tolower(conv);
    }
    switch (conv) {
    case 'd': case 'i': case 'o': case 'u': case 'x': {
      bool is_signed = false;
      if (conv == 'd' || conv == 'i') {
	is_signed = true;
      }
      // fetch numeric argument
      intmax_t num;
      if (rank > 0) {
	switch (lengthmod) {
	case 'h': {
	  num = va_arg(ap, int);
	  if (rank == 1) {
	    // fetch short int argument
	    if (is_signed) {
	      num = (short int) num;
	    } else {
	      num = (unsigned short int) num;
	    }
	  } else {
	    // fetch char argument
	    if (is_signed) {
	      num = (char) num;
	    } else {
	      num = (unsigned char) num;
	    }
	  }
	} break;
	case 'l': {
	  if (rank == 1) {
	    // fetch long int argument
	    if (is_signed) {
	      num = va_arg(ap, long int);
	    } else {
	      num = va_arg(ap, unsigned long int);
	    }
	  } else {
	    // fetch long long int argument
	    if (is_signed) {
	      num = va_arg(ap, long long int);
	    } else {
	      num = va_arg(ap, unsigned long long int);
	    }
	  }
	}
	case 'j': {
	  // fetch intmax_t argument
	  num = va_arg(ap, intmax_t);
	} break;
	case 'z': {
	  // fetch size_t argument
	  num = va_arg(ap, size_t);
	} break;
	case 't': {
	  // fetch ptrdiff_t argument
	  num = va_arg(ap, ptrdiff_t);
	} break;
	}
      } else {
	// fetch int argument
	num = va_arg(ap, int);
      }
      // print numeric argument
      int base = conv == 'o'? 8 : conv == 'x'? 16 : 10;
      int digits = 0;
      for (int div = num; div != 0; div /= base) {
	digits++;
      }
      // precision and field width. default precision is 1
      int zeros = (precision != -1? precision : 1) - digits;
      zeros = zeros > 0? zeros : 0;
      int pad = field_width - zeros - digits;
      pad = pad > 0? pad : 0;
      // calculate sign field
      char sign_field = 0;
      if (num < 0) {
	sign_field = '-';
      } else {
	if (plus) {
	  sign_field = '+';
	} else if (space) {
	  sign_field = ' ';
	}
      }
      if (sign_field != 0) {
	pad--;
      }
      // zero pad using field width specifier
      if (leading && precision == -1 && pad > 0) {
	zeros += pad;
	pad = 0;
      }
      // field width padding on left
      if (!left) {
	for (int pad_idx = 0; pad_idx < pad; pad_idx++) {
	  putchar(' ');
	}
      }
      // print sign
      if (sign_field != 0) {
	putchar(sign_field);
      }
      // zero pad from precision
      for (int zero_idx = 0; zero_idx < zeros; zero_idx++) {
	putchar('0');
      }
      // contsruct a string of converted digits
      char num_str[digits];
      for (int div = num, num_str_idx = digits - 1; div != 0; div /= base) {
	int digit = div % base;
	if (digit < 0) {
	  digit *= -1;
	}
	num_str[num_str_idx--] = (digit < 10? '0' : upper? 'A' : 'a') + digit;
      }
      // print the constructed string
      for (int num_str_idx = 0; num_str_idx < digits; num_str_idx++) {
	putchar(num_str[num_str_idx]);
      }
      // field width padding on right
      if (left) {
	for (int pad_idx = 0; pad_idx < pad; pad_idx++) {
	  putchar(' ');
	}
      }
    } break;
    case 'f': case 'e': case 'g': case 'a': {
      // XXX fetch double argument

      switch (conv) {
      case 'f': {
	// XXX print double in decimal
      } break;
      case 'e': {
	// XXX print double in scientific notation
      } break;
      case 'g': {
	// XXX print double in either decimal or scientific notation
      } break;
      case 'a': {
	// XXX print double in hexdecimal
      } break;
      }
    } break;
    case 'c': {
      // fetch char argument
      char ch = va_arg(ap, int);
      // print char
      putchar(ch);
    } break;
    case 's': {
      // fetch string argument
      const char *arg = va_arg(ap, const char *);
      size_t arg_len = precision != -1? strnlen(arg, precision) : strlen(arg);
      // pad string on left
      int pad = field_width - arg_len;
      pad = pad > 0? pad : 0;
      if (!left) {
	for (int pad_idx = 0; pad_idx < pad; pad_idx++) {
	  putchar(leading? '0' : ' ');
	}
      }
      // print string
      for (int arg_idx = 0; arg_idx < arg_len; arg_idx++) {
	putchar(arg[arg_idx]);
      }
      if (left) {
	for (int pad_idx = 0; pad_idx < pad; pad_idx++) {
	  putchar(' ');
	}
      }
    } break;
    case 'p': {
      // XXX fetch pointer argument

      // XXX print formatted pointer
    } break;
    case 'n': {
      // XXX write number of characters written to int argument
    } break;
    default: {
      va_end(ap);
      return -1;
    }
    }
  }
  va_end(ap);
  return chars;
}
