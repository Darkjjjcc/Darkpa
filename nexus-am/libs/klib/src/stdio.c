#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


void add_string(char **s, char *str);
void add_char(char *s, char c);
void add_number(char **s, int num);



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
  char *temp=out;
  while(*fmt!='\0'){
    if(*fmt!='%'){
      *temp++=*fmt++;
    }
    else{
      fmt++;
      switch(*fmt){
        case 's':{
          char *str=va_arg(ap,char*);
          add_string(&temp,str);
          break;
        }
        case 'd':{
          int n=va_arg(ap,int);
          add_number(&temp,n);
          break;
        }
      }
      fmt++;
    }
  }
  *temp='\0';
  return temp-out;
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


void add_string(char **s, char *str) {
  while (*str!='\0') {
    **s = *str;
    (*s)++;
    str++;
  }
}

void add_char(char *s, char c) {
  *s++ = c;
}

void add_number(char **s, int num) {
  if (num == 0) {
    add_char(*s, '0');
    (*s)++;
    return;
  }
  char temp[100];
  int i = 0;
  while (num) {
    temp[i++] = num % 10 + '0';
    num /= 10;
  }
  while(i--) {
    add_char(*s, temp[i]);
    (*s)++;
  }
}
#endif