#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {

  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
    char *str = out;
    const char *p = fmt;
    while (*p) {
        if (*p != '%') {
            *str++ = *p++;
            continue;
        }
        p++; // Skip '%'
        switch (*p) {
            case 'c': {
                char c = (char) va_arg(ap, int);
                *str++ = c;
                break;
            }
            case 'd':
            case 'i': {
                int i = va_arg(ap, int);
                str += sprintf(str, "%d", i);
                break;
            }
            case 'u': {
                unsigned int u = va_arg(ap, unsigned int);
                str += sprintf(str, "%u", u);
                break;
            }
            case 'x':
            case 'X': {
                unsigned int x = va_arg(ap, unsigned int);
                str += sprintf(str, "%x", x);
                break;
            }
            case 's': {
                const char *s = va_arg(ap, const char *);
                while (*s) {
                    *str++ = *s++;
                }
                break;
            }
            case 'p': {
                void *ptr = va_arg(ap, void *);
                str += sprintf(str, "%p", ptr);
                break;
            }
            case '%': {
                *str++ = '%';
                break;
            }
            default: {
                // Handle unknown format specifier
                *str++ = '%';
                *str++ = *p;
                break;
            }
        }
        p++;
    }
    *str = '\0';
    return str - out;
}

int sprintf(char *out, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = vsprintf(out, fmt, ap);
    va_end(ap);
    return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {

  return 0;
}

#endif
