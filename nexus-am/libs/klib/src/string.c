#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  for(size_t i = 0;;i++){
    if(s[i] == '\0') return i;
  }
  return 0;
}

char *strcpy(char* dst,const char* src) {
  for(size_t i = 0;;i++){
    dst[i] = src[i];
    if(src[i] == '\0') return dst;
  }
  return NULL;
}

char* strncpy(char* dst, const char* src, size_t n) {
  for(size_t i = 0;i < n;i++){
    dst[i] = src[i];
    if(src[i] == '\0') return dst;
  }
  return NULL;
}

char* strcat(char* dst, const char* src) {
  size_t len = strlen(dst);
  for(size_t i = 0;;i++){
    dst[len + i] = src[i];
    if(src[i] == '\0') return dst;
  }
  return NULL;
}

int strcmp(const char* s1, const char* s2) {
  for(size_t i = 0;;i++){
    if(s1[i] != s2[i]) return s1[i] - s2[i];
    if(s1[i] == '\0') return 0;
  }
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  for(size_t i = 0;i < n;i++){
    if(s1[i] != s2[i]) return s1[i] - s2[i];
    if(s1[i] == '\0') return 0;
  }
  return 0;
}

void* memset(void* v,int c,size_t n) {
  for(size_t i = 0;i < n;i++){
    ((char*)v)[i] = c;
  }
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  for(size_t i = 0;i < n;i++){
    ((char*)out)[i] = ((char*)in)[i];
  }
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n){
  for(size_t i = 0;i < n;i++){
    if(((char*)s1)[i] != ((char*)s2)[i]) return ((char*)s1)[i] - ((char*)s2)[i];
  }
  return 0;
}

#endif
