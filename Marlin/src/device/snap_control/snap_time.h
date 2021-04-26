#ifndef SNAP_TIMER_H
#define SNAP_TIMER_H

#include "../../common/common.h"
#include "snap_control_base.h"

class SnapTimer : public SnapControlBaes {
 public:
  static void DelayMs(uint32_t ms);
  static uint32_t Millis();
};

#endif
