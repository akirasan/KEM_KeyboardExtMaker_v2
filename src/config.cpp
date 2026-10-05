// Configuración por defecto (equivale a tu keys_config.h actual).
// En la fase 2 se cargará desde flash y esto quedará como "reset de fábrica".
#include <Arduino.h>
#include <string.h>
#include "types.h"
#include "macro.h"
#include "hid.h"
#include "config.h"

Config g_cfg;

#define RGB(r, g, b) (((uint32_t)(r) << 16) | ((uint32_t)(g) << 8) | (uint32_t)(b))

namespace {

void setKey(uint8_t i, uint8_t mode, const char *name, uint32_t idle, uint32_t press) {
  KeyConfig &k = g_cfg.keys[i];
  k.mode = mode;
  strncpy(k.name, name, KEY_NAME_LEN - 1);
  k.name[KEY_NAME_LEN - 1] = 0;
  k.color_idle = idle;
  k.color_press = press;
}

} // namespace

namespace config {

void loadDefaults() {
  memset(&g_cfg, 0, sizeof(g_cfg));
  g_cfg.base_color = RGB(50, 0, 150);
  g_cfg.brightness = 120;

  // KEY1: Alt+Tab
  setKey(0, MODE_TAP, "Alt + TAB", RGB(5, 5, 5), RGB(50, 50, 50));
  macroTap(g_cfg.keys[0].on_press, MOD_LALT, HID_TAB);

  // KEY2: Captura recortes (Ctrl+Win+S)
  setKey(1, MODE_TAP, "Recorte", RGB(50, 0, 50), RGB(50, 0, 150));
  macroTap(g_cfg.keys[1].on_press, MOD_LSHIFT | MOD_LGUI, HID_LETTER('s'));

  // KEY3, KEY5, KEY6: sin acción (iluminan al pulsar, para validar el hardware)
  setKey(2, MODE_NONE, "KEY 3", RGB(0, 0, 20), RGB(0, 0, 255));
  setKey(4, MODE_NONE, "KEY 5", RGB(0, 0, 20), RGB(0, 0, 255));
  setKey(5, MODE_NONE, "KEY 6", RGB(0, 0, 20), RGB(0, 0, 255));

  // KEY4: Alt+F4
  setKey(3, MODE_TAP, "Alt + F4", RGB(55, 0, 0), RGB(55, 55, 0));
  macroTap(g_cfg.keys[3].on_press, MOD_LALT, HID_FKEY(4));

  // KEY7: ESC
  setKey(6, MODE_TAP, "ESC", RGB(5, 5, 5), RGB(255, 0, 0));
  macroTap(g_cfg.keys[6].on_press, 0, HID_ESC);

  // KEY8: AFK (flecha arriba/abajo cada 5 s mientras está activo)
  setKey(7, MODE_AFK, "AFK", 0, RGB(100, 100, 100));
  KeyConfig &afk = g_cfg.keys[7];
  afk.afk_seconds = 5;
  afk.color_on  = RGB(0, 55, 0);
  afk.color_off = RGB(55, 0, 0);
  macroTap(afk.on_press, 0, HID_UP);
  macroDelay(afk.on_press, 100);
  macroTap(afk.on_press, 0, HID_DOWN);
}

} // namespace config
