#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(char* expre) {
  if (free_ == NULL) { 
    assert(0);
    return NULL;
  }
  WP* res = free_;
  free_ = free_->next;
  res->next = head;
  head = res;

  if(strlen(expre) >= strlen(res->expr)) {assert("expression too long");}
  strcpy(res->expr, expre);

  bool success = true;
  res->nowValue = res->oldValue = expr(res->expr, &success);
  if(!success) {assert("Wrong expression");}

  return res;
}

bool free_wp(int No){
  WP* p = head;
  WP* pre = NULL;
  while(p != NULL) {
    if(p->NO == No) {
      if(pre == NULL) {
        head = p->next;
      }
      else {
        pre->next = p->next;
      }
      p->next = free_;
      free_ = p;
      return true;
    }
    pre = p;
    p = p->next;
  }
  return false;
}

void watchpoint_display() {
  WP* p = head;
  while(p != NULL) {
    printf("watchpoint %d: %s\n", p->NO, p->expr);
    printf("old value: %u\n", p->oldValue);
    printf("new value: %u\n", p->nowValue);
    p = p->next;
  }
}

bool check_wp() {
  WP* p = head;
  bool flag = false;
  while(p != NULL) {
    bool success = true;
    p->nowValue = expr(p->expr, &success);
    if(!success) {assert("Wrong expression");}
    if(p->nowValue != p->oldValue) {
      flag = true;
      printf("watchpoint %d: %s\n", p->NO, p->expr);
      printf("old value: %u\n", p->oldValue);
      printf("new value: %u\n", p->nowValue);
      p->oldValue = p->nowValue;
    }
    p = p->next;
  }
  return flag;
}


