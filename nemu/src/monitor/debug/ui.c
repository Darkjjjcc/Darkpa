#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

extern void isa_reg_display(void);

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
  char *steps = strtok(args, " ");
  int n=1;
  if(steps != NULL){
    n = atoi(steps);      // ascii to integer
  }
  cpu_exec(n);
  return 0;
}

static int cmd_info(char *args){
  char *arg = strtok(NULL, " ");
  if(arg == NULL){
    printf("Unknown command, please input the subcmd!\n");
  }
  else if(strcmp(arg, "r") == 0){
    isa_reg_display();
  }
  else if(strcmp(arg, "w") == 0){
    watchpoint_display();
  }
  else{
    printf("Unknown command, please check the subcmd!\n");
  }
  return 0;
}

static int cmd_x(char *args) { 
  char *N = strtok(NULL, " ");
  char *arg = strtok(NULL, " ");
  if(N == NULL || arg == NULL){
    printf("Unknown command, please input the N and the address!\n");
    return 0;
  }
  int n = atoi(N);
    paddr_t addr = strtol(arg, NULL, 16);
    for(int i=0; i<n; i++) {
        printf("0x%08x: ", addr);
        for(int j=0; j<4; j++) {
            printf("%02x ", paddr_read(addr, 1));
            addr++;
        }
        printf("\n");
    }
    return 0;
}

static int cmd_p(char *args) {
  char *arg = strtok(NULL, " ");
  bool success = true;
  uint32_t result = expr(arg, &success);
  if(success) {
    printf("%s = %u\n", arg, result);
  }
  else {
    printf("Invalid expression!\n");
  }
  return 0;
}

static int cmd_w(char *args) {
  char *arg = strtok(NULL, " ");
  if(arg == NULL){
    printf("Unknown command, please input the expression!\n");
    return 0;
  }
  WP* wp = new_wp(arg);
  printf("Set watchpoint %d for %s\n", wp->NO, wp->expr);
  return 0;
}

static int cmd_d(char *args) {
  char *arg = strtok(NULL, " ");
  if(arg == NULL){
    printf("Unknown command, please input the watchpoint number!\n");
    return 0;
  }
  int n = atoi(arg);
  if(free_wp(n)){
    printf("Delete watchpoint %d\n", n);
  }
  else{
    printf("No watchpoint %d\n", n);
  }
  return 0;
}

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Execute N instructions in a single step", cmd_si},
  { "info", "Print the state of the program",cmd_info},
  { "x", "Scan memory", cmd_x},
  { "p", "Evaluate the expression", cmd_p},
  { "w", "Set watchpoint", cmd_w},
  { "d", "Delete watchpoint", cmd_d},


  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}