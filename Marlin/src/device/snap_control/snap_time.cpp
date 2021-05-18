#include "snap_time.h"
#include <wirish_time.h> 
#include "../snap_star_parse.h"

void SnapTimer::DelayMs(uint32_t ms) {
  uint32_t until = Millis() + ms;
  while (until > Millis()) {
    canbus_g.Handler();
    registryInstance.ConfigHandler();
    registryInstance.ServerHandler();
    registryInstance.SystemHandler();
    sys_uart.Loop();
    snap_parse.loop();
  }
}

uint32_t SnapTimer::Millis() {
  return millis();
}


