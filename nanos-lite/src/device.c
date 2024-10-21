#include "common.h"
#include <amdev.h>

extern int screen_width();
extern int screen_height();

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for (size_t i = 0; i < len; i++) {
    _putc(((char *)buf)[i]);
  }
  return 0;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  int kc = read_key();
  char tmp[3] = "ku";
  if ((kc & 0xfff) == _KEY_NONE) {
    int time = uptime();
    len = sprintf(buf, "t %d\n", time);
  }
  else {
    if (kc & 0x8000)
      tmp[1] = 'd';
    len = sprintf(buf, "%s %s\n", tmp, keyname[kc & 0xfff]);
  }
  return len;
}

static char dispinfo[128] __attribute__((used)) = {};
static int screen_h, screen_w;

size_t get_dispinfo_size() {
  return strlen(dispinfo);
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  strncpy(buf, dispinfo + offset, len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  // _yield(); // 模拟IO慢，进行调度

  int x, y;
  assert(offset + len <= (size_t)screen_h * screen_w * 4);
  x = (offset / 4) % screen_w;
  y = (offset / 4) / screen_w;
  assert(x + len < (size_t)screen_w * 4);
  draw_rect((void *)buf, x, y, len / 4, 1);
  return len;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  //fb_write(buf, offset, len);
  draw_sync();
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // XXX: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  screen_h = screen_height();
  screen_w = screen_width();
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", screen_w, screen_h);
}