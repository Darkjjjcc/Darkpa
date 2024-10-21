#include "common.h"
#include "syscall.h"
// PA3.3 updated
#include "fs.h"
#include "proc.h"

void sys_yield();
void sys_exit(int code);
int sys_write(int fd, void *buf, size_t count);
int sys_brk(intptr_t addr);
static int program_break;
// PA3.4 updated
int sys_open(const char *pathname, int flags, int mode);
int sys_read(int fd, void *buf, size_t count);
int sys_lseek(int fd, size_t offset, int whence);
int sys_close(int fd);
// PA3.4 updated
int sys_execve(const char *pathname, char *const argv[], char *const envp[]);


_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  // printf("GPR1: %d\n", a[0]);
  // printf("GPR2: %d\n", a[1]);
  // printf("GPR3: %d\n", a[2]);
  // printf("GPR4: %d\n", a[3]);

  switch (a[0]) {
    case SYS_yield:
      sys_yield();
      c->GPRx = 0;
      break;
    case SYS_exit:
      sys_exit(a[1]);
      break;
    case SYS_write:
      c->GPRx = sys_write(a[1], (void*)(a[2]), a[3]);
      break;
    case SYS_brk:
      c->GPRx = sys_brk(a[1]);
      break;
    case SYS_open:
      c->GPRx = sys_open((const char*)a[1], a[2], a[3]);
      break;
    case SYS_read:
      c->GPRx = sys_read(a[1], (void*)(a[2]), a[3]);
      break;
    case SYS_lseek:
      c->GPRx = sys_lseek(a[1], a[2], a[3]);
      break;
    case SYS_close:
      c->GPRx = sys_close(a[1]);
      break;
    case SYS_execve:
      printf("%s\n", a[1]);
      sys_execve((const char*)a[1], NULL, NULL);
      c->GPR2 = SYS_exit;
      do_syscall(c);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}

void sys_yield() {
  _yield();
}

void sys_exit(int code) {
  // _halt(code);
  return sys_execve("/bin/init", NULL, NULL);
}


int sys_write(int fd, void *buf, size_t count) {
  // if(fd == 1 || fd == 2) {
  //   for(int i = 0; i < count; i++) {
  //     _putc(((char*)buf)[i]);
  //   }
  //   return count;
  // }
  // return 0;
  return fs_write(fd, buf, count);
}

int sys_brk(intptr_t addr) {
  program_break = addr;
  return 0;
}

int sys_open(const char *pathname, int flags, int mode) {
  return fs_open(pathname, flags, mode);
}

int sys_read(int fd, void *buf, size_t count) {
  return fs_read(fd, buf, count);
}

int sys_lseek(int fd, size_t offset, int whence) {
  return fs_lseek(fd, offset, whence);
}

int sys_close(int fd) {
  return fs_close(fd);
}

int sys_execve(const char *pathname, char *const argv[], char *const envp[]) {
  naive_uload(NULL, pathname);
  return 0;
}