#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char expr[65536];
  bool valueChanged;
  uint32_t oldValue,nowValue;

} WP;

WP* new_wp(char* expre);
bool free_wp(int No);
void watchpoint_display();
bool check_wp();

#endif
