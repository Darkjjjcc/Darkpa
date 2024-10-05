#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char outBuf[256] = {'\0'};
  int length = vsprintf(outBuf, fmt, args); 
  for (size_t i = 0; outBuf[i]; i++) {
    _putc(outBuf[i]);
  }
  va_end(args);
  return length;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
    int count = 0, flag = 0, fmtnum = 0;
    char tmpc[30], sflag;
    char *tmp;
    int inte;
    int i = 0, j = 0, k;

    for(;fmt[i];i++){
        if(flag){
            flag = 0;
            if(fmt[i]>='0' && fmt[i]<='9'){
                fmtnum *= 10;
                fmtnum += fmt[i]-'0';
                flag = 1;
                continue;
            }
            switch(fmt[i]){
                case 's':
                    tmp = va_arg(ap,char*);
                    if(fmtnum > strlen(tmp)){
                        for(int a = 0; a < fmtnum-strlen(tmp);a++){
                            out[j] = ' ';
                            j++;
                        }
                    }
                    strcat(out+j, tmp);
                    j += strlen(tmp);
                    count++;
                    break;
                case 'd':
                    inte = va_arg(ap, int);
                    k = 0;
                    sflag = inte < 0;
                    inte = inte *(sflag?-1:1);
                    if(sflag){
                        out[j] = '-';
                        j++;
                    }
                    do{
                        tmpc[k] = inte % 10;
                        inte /= 10;
                        k++;
                    }while(inte);
                    if(fmtnum > k){
                        for(int l = 0; l < fmtnum-k;l++){
                            out[j] = ' ';
                            j++;
                        }
                    }
                    for(int l = 0; l < k;l++){
                        out[j+l] = tmpc[k-1-l] + '0';
                    }
                    j+=k;
                    count++;
                    break;
                    //TODO: deal with '%'
                default:
                    assert(0 && "Unfinish stdio");
            }
            continue;
        }
        if(fmt[i] == '%'){
            flag = 1;
            fmtnum = 0;
            continue;
        }
        out[j] = fmt[i];
        j++;
    }
    out[j] = 0;
    return count;
}

int sprintf(char *out, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int count = vsprintf(out, fmt, ap);
    va_end(ap);
    return count;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif