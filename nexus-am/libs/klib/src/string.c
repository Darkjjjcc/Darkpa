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
  // 将 void* 指针转换为 unsigned char* 指针，以便按字节访问  
  const unsigned char *p1 = (const unsigned char *)s1;  
  const unsigned char *p2 = (const unsigned char *)s2;  

  // 遍历指定的字节数  
  while (n--) {  
      // 比较当前字节  
      if (*p1 != *p2) {  
          // 如果发现不同的字节，则返回它们的差值（按无符号字符计算）  
          // 注意：这里实际上返回的是差值的符号，因为 C 语言中整数运算的符号扩展  
          return (int)(*p1 - *p2);  
      }  
      // 移动到下一个字节  
      p1++;  
      p2++;  
  }  
  // 如果所有比较的字节都相同，则返回 0 
  return 0;
}

#endif
