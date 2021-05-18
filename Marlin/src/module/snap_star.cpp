#include "snap_star.h"
#include "../device/snap_control.h"
#include "../device/snap_star_parse.h"
#include "../common/common.h"
#include <wirish_time.h>

static SnapStartRecv_t recv_data;

void SnapStar::Init() {
  control_.Init();
  user_.setup();
}

void RecvDataPush(uint8_t * data, uint8_t data_len) {
  if (data_len && data) {
    for (uint8_t i = 0; i < data_len && (recv_data.len < SNAP_STAR_MAX_RECV); i++) {
      recv_data.data[recv_data.len++] = data[i];
    }
  }
}

bool RecvDataCheck() {
  return true;
}

void RecvDataExecute() {
  recv_data.data[recv_data.len++] = 0;
  if (recv_data.mode == SNAP_DATA_STRING_MODE) {
    snap_parse.ParamAndExecute(recv_data.data, SNAP_DATA_STRING_MODE, SNAP_CMD_SOUREC_CAN);
  } else if (recv_data.mode == SNAP_DATA_API_MODE) {
    snap_parse.ParamAndExecute(recv_data.data, SNAP_DATA_API_MODE, SNAP_CMD_SOUREC_CAN);
  }
}

void RecvDataPares(uint8_t * data, uint8_t data_len) {
  if (data_len == 0) {
    return ;
  }
  SnapDataHead_t *head = (SnapDataHead_t *)(data);
  switch (head->type) {
    case SNAP_DATA_PACK_START:
      recv_data.mode = head->mode;
      recv_data.check = data[1] << 8 | data[2];
      recv_data.time = millis();
      recv_data.len = 0;
      RecvDataPush(&data[3], data_len - 3);
      break;

    case SNAP_DATA_PACK_DATA:
      RecvDataPush(&data[1], data_len - 1);
      break;

    case SNAP_DATA_PACK_END:
      RecvDataPush(&data[1], data_len - 1);
      if (RecvDataCheck()) {
        RecvDataExecute();
      }
      break;

    case SNAP_DATA_PACK_SIGLE:
      recv_data.mode = head->mode;
      recv_data.len = 0;
      RecvDataPush(&data[1], data_len - 1);
      RecvDataExecute();
      break;
  }

}

void SnapStar::HandModule(uint16_t func_id, uint8_t * data, uint8_t data_len) {
  switch (func_id) {
    case FUNC_SNAP_STAR_CORTROL:
      RecvDataPares(data, data_len);
      break;

  }
}

void SnapStar::Loop() {
  control_.Loop();
  snap_parse.loop();
  user_.loop();
}
