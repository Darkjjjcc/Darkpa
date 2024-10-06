#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void add_string(char *s, char *str) {
  while (*str) {
    *s++ = *str++;
  }
}

void add_char(char *s, char c) {
  *s++ = c;
}

void add_number(char *s, int num) {
  if (num == 0) {
    add_char(s, '0');
    return;
  }
  char buf[100];
  int i = 0;
  if(num < 0){
    add_char(s, '-');
    num = -num;
  }
  while (num) {
    buf[i++] = num % 10 + '0';
    num /= 10;
  }
  while (i)
    add_char(s, buf[--i]);
}


int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);
  char buf[1000];
  int length=vsprintf(buf,fmt,ap);
  buf[length]='\0';
  for(int i=0;i<length;i++){
    _putc(buf[i]);
  }
  va_end(ap);
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *s = out;
  int len = 0;
  while(*fmt) {
    if(*fmt == '%') 
      *s++ = *fmt++;
    else {
      fmt++;
      switch(*fmt) {
        case 's': {
          char *str = va_arg(ap, char *);
          add_string(s, str);
          break;
        }
        case 'd': {
          int n=va_arg(ap,int);
          if(n==0){
            *s++='0';
            break;
          }
          if(n<0){
            *s++='-';
            n=-n;
          }
          char buf[11];
          int i=0;
          while(n!=0){
            buf[i++]=n%10+'0';
            n=n/10;
          }
          for(int j=i-1;j>=0;j--){
            *s++=buf[j];
          }
          break;
        }
        default: {
          add_char(s, *fmt);
          break;
        }
      }
    } 
    fmt++;
  }
  *s = '\0';
  return s - out;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);
  int length=vsprintf(out,fmt,ap);
  va_end(ap);
  return length;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif