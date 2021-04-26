/*
 * Snapmaker2-Modules Firmware
 * Copyright (C) 2019-2020 Snapmaker [https://github.com/Snapmaker]
 *
 * This file is part of Snapmaker2-Modules
 * (see https://github.com/Snapmaker/Snapmaker2-Modules)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SRC_MODULE_SNAP_STAR_H_
#define SRC_MODULE_SNAP_STAR_H_

#include "src/device/switch.h"
#include "src/configuration.h"
#include "module_base.h"
#include "../HAL/hal_include.h"
#include "snap_star_user.h"

#define SNAP_STAR_MAX_RECV 60

typedef struct {
  uint16_t check;
  uint8_t  mode;
  uint8_t  len;
  uint32_t time;
  uint8_t data[SNAP_STAR_MAX_RECV];
} SnapStartRecv_t;

class SnapStar : public ModuleBase {

 public:
  void Init();
  void HandModule(uint16_t func_id, uint8_t * data, uint8_t data_len);
  void Loop();

 private:
  SnapStarUser user_;
};

#endif //SRC_MODULE_SNAP_STAR_H_
