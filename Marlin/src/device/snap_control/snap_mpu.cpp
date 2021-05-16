#include "snap_mpu.h"
#include "snap_time.h"
#include "snap_io.h"

ErrCode ReadQuaternion(Quaternion &quaternion) {
  uint32_t timeout = SnapTimer::Millis() + 1000;
  float q[4];
  while (mpu_get_quaternion(q) != 0) {
    SnapTimer::DelayMs(1);
    if (timeout < SnapTimer::Millis()) {
      return E_FAILURE;
    }
  }
  quaternion = QuaterCreat(q[0], q[1], q[2], q[3]);
  return E_SUCCESS;
}

ErrCode SnapMPU::Init(uint8_t sda_io, uint8_t scl_io, uint8_t addr) {
  if ((sda_io < IOxx) && (scl_io < IOxx)) {
    if (MPU_Init(snap_io_remap[sda_io], snap_io_remap[scl_io], addr) == E_SUCCESS) {
      if (Zero() == E_SUCCESS) {
        inited_ = true;
        return E_SUCCESS;
      }
    }
  }
  return E_FAILURE;
}

ErrCode SnapMPU::Zero() {
  return ReadQuaternion(init_q_);
}

ErrCode SnapMPU::Read(ValueOfMPU_t &value) {
  if (!inited_) {
    return E_FAILURE;
  }
  float xyz[3];
  Quaternion cur;
  if (ReadQuaternion(cur) != E_SUCCESS) {
    return E_FAILURE;
  }
  Quaternion diff = QuaterDiff(cur, init_q_);
  QuaterToEuler(diff, xyz);
  value = *((ValueOfMPU_t *)xyz);
  return E_SUCCESS;
}
