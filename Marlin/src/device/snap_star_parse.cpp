#include "snap_star_parse.h"
#include "../device/snap_control.h"
#include "../device/snap_control/snap_control_base.h"
#include "../common/common.h"
#include <string.h>
#include <stdio.h>


static SnapCommonParam_t str_param;

static ErrCode ParseStringParam(uint8_t *cmd, SnapCommonParam_t &param) {
  if (cmd == NULL) {
    return E_PARAM;
  }

  uint16_t i = 0;
  int32_t num = 0;
  param.api_type = API_INVALID;
  param.sub_num = SUB_NUM_INVALID;
  param.count = 0;
  param.write_index = 0;
  // Remove characters that are not letters
  for (; !IS_LETTER(*cmd) && (*cmd != '\0'); cmd++);

  if (IS_LETTER(*cmd)) {
    for (uint8_t t = 0; t < API_INVALID; t++) {
      if (IsBeginWith(cmd, (uint8_t *)name_to_api_table[t])) {
        cmd += strlen((char *)name_to_api_table[t]);
        if ((*cmd == '.') || (*cmd == '_')) {
          if (StringToInt(++cmd, num)) {
            param.sub_num = num;
          }
        }
        param.api_type = (API_NUM_E)t;
        break;
      }
    }
  }

  if (param.api_type >= API_INVALID) {
    return E_FAILURE;
  } else if (param.api_type == API_GCODE) {
    param.count = 1;
    param.param_type[0] = PARAM_ADDRESS;
    param.value[0].p = cmd + 1;
    return E_SUCCESS;
  }

  while (((*cmd) != ' ') && ((*cmd) != '\0')) {
    cmd++;
  }

  while ((*cmd) != '\0' && (param.count < MAX_PARAM_COUNT)) {
    if (IS_NUMBER(*cmd)) {
      if (IfStringFLoat(cmd)) {
        param.param_type[param.count] = PARAM_FLOAT;
        StringToFloat(cmd, param.value[param.count].f);
      } else {
        param.param_type[param.count] = PARAM_INT;
        StringToInt(cmd, param.value[param.count].i);
      }
      param.count++;
      while(IS_NUMBER(*cmd) || (*cmd == '.')) cmd++;
    } else if(IS_LETTER(*cmd)) {
      param.param_type[param.count] = PARAM_STRING;
      i = 0;
      while ((*cmd) != ' ' && ((*cmd) != '\0')) {
        param.value[param.count].s[i++] = (*cmd);
        cmd++;
      }
      param.value[param.count].s[i] = 0;
      param.count++;
    } else if (*cmd == '=' && !param.write_index) {
      param.param_type[param.count] = PARAM_WRITE_MARK;
      param.count++;
      param.write_index = param.count;
      cmd++;
    } else {
      cmd++;
    }
  }
  if (param.write_index && param.write_index >= param.count) {
    param.api_type = API_INVALID;
    return E_FAILURE;
  }

  return E_SUCCESS;
}

ErrCode SnapStarParse::ParamAndExecute(uint8_t *cmd, SNAP_CMD_TYPE_E cmd_type, SNAP_CMD_SOUREC_E source) {
  uint8_t api_type;
  SnapApiParam_t *api_param = (SnapApiParam_t *)cmd;
  SnapApiRetuen_t ret = {E_FAILURE};
  SnapControlBaes * snap_control = NULL;
  void * param = NULL;
  void * snap_ret = NULL;
  if (cmd_type == SNAP_DATA_STRING_MODE) {
    ToLowers(cmd);
    ParseStringParam(cmd, str_param);
    api_type = str_param.api_type;
  } else {
    api_type = api_param->api_type;
  }

  #define InitVariable(api)  \
      snap_control = &control_.api; \
      param = &api_param->api; \
      snap_ret = &ret.api;

  switch (api_type) {
    case API_INDEX:
      InitVariable(index);
      break;
    case API_IO:
      InitVariable(io);
      break;
    case API_ADC:
      InitVariable(adc);
      break;
    case API_UART:
      break;
    case API_PWM:
      InitVariable(pwm);
      break;
    case API_TEMP:
      // ret = ParseTemperature(cmd, sub_num);
      break;
    case API_GCODE:
      InitVariable(gcode);
      break;
    case API_MPU:
      InitVariable(mpu);
      break;
    case API_INVALID:
      break;
  }

  if (!snap_control || !param || ! snap_ret) {
    return E_FAILURE;
  }

  switch (cmd_type) {
    case SNAP_DATA_API_MODE:
      snap_control->ApiControl(param, snap_ret);
      break;
    case SNAP_DATA_STRING_MODE:
      snap_control->StrControl(str_param, snap_ret);
      break;
  }

  if (source == SNAP_CMD_SOUREC_UART) {
    snap_control->ShowResult(snap_ret);
  } else {
    snap_control->AckControl(snap_ret);
  }
  return E_SUCCESS;
}

void SnapStarParse::loop() {
  uint16_t cmd_len = 0;
  uint8_t * cmd = NULL;
  if (lock_) {
    return;
  }
  if ((cmd = sys_uart.CmdPop(cmd_len)) != NULL) {
    lock_ = true;
    sys_uart.CleanCmd();
    ParamAndExecute(cmd, SNAP_DATA_STRING_MODE, SNAP_CMD_SOUREC_UART);
    lock_ = false;
  }
}

SnapStarParse snap_parse;
