#include <am.h>
#include <amdev.h>
#include <nemu.h>

static uint32_t boot_time = 0;

size_t __am_timer_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_TIMER_UPTIME: {
      _DEV_TIMER_UPTIME_t *uptime = (_DEV_TIMER_UPTIME_t *)buf;
      // PA2.3 TODO: calculate time using RTC (Real Time Counter)
      uint32_t current_time = inl(RTC_ADDR);
      uptime->hi = 0;
      uptime->lo = current_time;
      return sizeof(_DEV_TIMER_UPTIME_t);
    }
    case _DEVREG_TIMER_DATE: {
      _DEV_TIMER_DATE_t *rtc = (_DEV_TIMER_DATE_t *)buf;
      rtc->second = 0;
      rtc->minute = 0;
      rtc->hour   = 0;
      rtc->day    = 0;
      rtc->month  = 0;
      rtc->year   = 2000;
      return sizeof(_DEV_TIMER_DATE_t);
    }
  }
  return 0;
}

void __am_timer_init() {
  // PA2.3 TODO: initialize RTC
  boot_time = inl(RTC_ADDR);
}
