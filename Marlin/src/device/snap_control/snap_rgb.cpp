#include "snap_rgb.h"

ErrCode SnapRGB::Set(SNAP_PIN_REMEP io, RGB_T rgb, uint8_t count) {
  if (io_ != io || rgb_count != count) {
    rgb_.Init(snap_io_remap[io], count, SOFT_EXTI_LINE1);
    io_ = io;
    rgb_count = count;
  }
  rgb_.StaticLight(rgb);
  return E_SUCCESS;
}

ErrCode SnapRGB::Breathe(SNAP_PIN_REMEP io, RGB_T rgb, uint8_t count, uint32_t breathe_period_ms) {
  if (io_ != io || rgb_count != count) {
    rgb_.Init(snap_io_remap[io], count, SOFT_EXTI_LINE1);
  }
  rgb_.BreathLight({0,0,0}, rgb, breathe_period_ms);
  return E_SUCCESS;
}

void SnapRGB::Loop() {
  rgb_.Loop();
}