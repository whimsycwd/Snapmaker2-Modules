#include "snap_control_base.h"

static uint8_t snap_send_buf[PUBLIC_BUF_SIZE];

const uint8_t name_to_api_table[][8] = {
  "index",// API_INDEX
  "io",   // API_IO
  "adc",  // API_ADC
  "uart", // API_UART
  "pwm",  // API_PWM
  "temp", // API_PWM
  "gcode", // API_GODE
  {"invalid"},   // API_INVALID
};

ErrCode SendToContorl(API_NUM_E api, void * info, uint8_t len) {
  uint8_t buf[8] = {0};
  uint8_t * data = (uint8_t *)info;
  SnapDataHead_t *head = (SnapDataHead_t *)buf;
  uint8_t i=0, length=0;

  #define PACK_DATA_SIZE 7
  uint16_t msgid = registryInstance.FuncId2MsgId(FUNC_SNAP_STAR_CORTROL);
  if (msgid == INVALID_VALUE) {
    return E_INIT;
  }

  snap_send_buf[0] = api;
  StringCopy(&snap_send_buf[1], data, len++);
  data = snap_send_buf;

  head->mode = SNAP_DATA_API_MODE;
  if (len <= PACK_DATA_SIZE) {
    // Single packet sending less than 7 bytes
    head->type = SNAP_DATA_PACK_SIGLE;
    length = len + 1;
    for (i = 0; i < len; i++) {
      buf[i+1] = data[i];
    }
    canbus_g.PushSendStandardData(msgid, buf, length);
  } else {
    // send start packet
    head->type = SNAP_DATA_PACK_START;
    buf[1] = 0;
    buf[2] = 0;
    length = 3;
    canbus_g.PushSendStandardData(msgid, buf, length);
    // send data
    while (len) {
      if (len / PACK_DATA_SIZE) {
        head->type = SNAP_DATA_PACK_DATA;
        for (i = 0; i < PACK_DATA_SIZE; i++)
          buf[i+1] = data[i];
        length = PACK_DATA_SIZE+1;
        len -= PACK_DATA_SIZE;
        data += PACK_DATA_SIZE;
      } else {
        head->type = SNAP_DATA_PACK_END;
        for (i = 0; i < len; i++)
          buf[i+1] = data[i];
        length = len + 1;
        len = 0;
      }
      canbus_g.PushSendStandardData(msgid, buf, length);
    }
  }
  return E_SUCCESS;
}
