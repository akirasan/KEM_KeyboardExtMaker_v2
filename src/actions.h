#pragma once
#include <stdint.h>
#include "input.h"
#include "types.h"

namespace actions {
  void begin();
  void handle(const KeyEvent &ev);   // despacha un evento según g_cfg
  void update();                     // avanza el reproductor de macros y los timers AFK
  bool isOn(uint8_t key);            // estado TOGGLE/AFK
  void play(const Macro *m);         // encola una macro
  bool busy();
  uint8_t dropped();
}
