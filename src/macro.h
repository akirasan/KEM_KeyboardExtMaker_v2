#pragma once
#include "types.h"

inline void macroClear(Macro &m) { m.len = 0; }

inline bool macroAdd(Macro &m, uint8_t type, uint8_t code = 0, uint16_t arg = 0) {
  if (m.len >= MACRO_MAX_STEPS) return false;
  Step &s = m.steps[m.len++];
  s.type = type; s.code = code; s.arg = arg;
  return true;
}

inline bool macroTap(Macro &m, uint8_t mods, uint8_t code) { return macroAdd(m, STEP_TAP, code, mods); }
inline bool macroDelay(Macro &m, uint16_t ms)              { return macroAdd(m, STEP_DELAY, 0, ms); }
