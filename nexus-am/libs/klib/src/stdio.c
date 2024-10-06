#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


void add_string(char **s, char *str);
void add_char(char **s, char c);
void add_number(char **s, int num);
void add_special_number(char **s, const char *fmt, va_list ap,char c);

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
      switch(*fmt){
        case 's':{
          char *str=va_arg(ap,char*);
          add_string(&temp,str);
          break;
        }
        case 'd':{
          int num=va_arg(ap,int);
          add_number(&temp,num);
          break;
        }
        case '0':{
          fmt++;
          add_special_number(&temp,fmt,ap,'0');
          fmt++;
          break;
        }
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':{
          add_special_number(&temp,fmt,ap,' ');
          fmt++;
          break;
        }
        case 'c':{
          char c=va_arg(ap,int);
          add_char(&temp,c);
          break;
        }
        case 'x':{
          int n=va_arg(ap,int);
          if(n==0) add_string(&temp,"0x0");break;
          if (n < 0) {
            *temp++ = '-';
            n = -n;
          }
          char buf[12];
          int i = 0;
          while (n != 0) {
            int a = n % 16;
            if (a < 10)
              buf[i++] = a + '0';
            else
              buf[i++] = a - 10 + 'a';
            n = n / 16;
          }
          *temp++ = '0';
          *temp++ = 'x';
          for (int j = i - 1; j >= 0; j--) {
            *temp++ = buf[j];
          }
          break;
        }
        default:{
          *temp++=*fmt;
          add_char(&temp,*fmt);
          break;
      }
    }
    fmt++;
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

void add_char(char **s, char c) {
  **s = c;
  (*s)++;
}

void add_number(char **s, int num) {
  if (num == 0) {
    add_char(s, '0');
    return;
  }
  char temp[100];
  int i = 0;
  if(num<0){
    add_char(s,'-');
    num=-num;
  }
  while (num) {
    temp[i] = num % 10 + '0';
    num /= 10;
    i++;
  }
  for (int j = i - 1; j >= 0; j--) {
    add_char(s, temp[j]);
  }
}


void add_special_number(char **s, const char *fmt, va_list ap,char c) {
  int num_of_digit=0;
  while(*fmt!='d'){
    num_of_digit=num_of_digit*10+*fmt-'0';
    fmt++;
  }
  int num=va_arg(ap,int);
  char temp[100];
  int i = num==0?1:0;
  while (num) {
    i++;
    num /= 10;
  }
  int x = num_of_digit - i;
  for (int j = 0; j < x; j++) {
    add_char(s, c);
  }
  add_number(s,num);
}

#endif