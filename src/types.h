#pragma once
#include <stdint.h>
#include "pins.h"

#define MACRO_MAX_STEPS 32
#define KEY_NAME_LEN    17

enum StepType : uint8_t {
  STEP_END = 0,
  STEP_DOWN,        // code: usage a pulsar (se mantiene hasta STEP_UP / RELEASE_ALL)
  STEP_UP,          // code: usage a soltar
  STEP_TAP,         // code: usage, arg: máscara de modificadores (pulsa y suelta)
  STEP_DELAY,       // arg: milisegundos (no bloquea)
  STEP_RELEASE_ALL
};

struct Step {
  uint8_t  type;
  uint8_t  code;
  uint16_t arg;
};

struct Macro {
  uint8_t len;
  Step    steps[MACRO_MAX_STEPS];
};

enum KeyMode : uint8_t {
  MODE_NONE = 0,   // sin acción
  MODE_TAP,        // PRESS -> on_press ; RELEASE -> on_release (opcional, para "mantener")
  MODE_TOGGLE,     // cada pulsación alterna: on_press (ON) / on_alt (OFF)
  MODE_AFK         // como TOGGLE, y en ON repite on_press cada afk_seconds
};

struct KeyConfig {
  uint8_t  mode;
  uint16_t afk_seconds;
  char     name[KEY_NAME_LEN];
  uint32_t color_idle;    // 0xRRGGBB, MODE_TAP/NONE en reposo
  uint32_t color_press;   // mientras la tecla está físicamente pulsada
  uint32_t color_on;      // TOGGLE/AFK activo
  uint32_t color_off;     // TOGGLE/AFK inactivo
  Macro    on_press;
  Macro    on_release;
  Macro    on_alt;
};

struct Config {
  KeyConfig keys[NUM_KEYS];
  uint32_t  base_color;
  uint8_t   brightness;
};

extern Config g_cfg;
