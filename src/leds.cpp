#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "leds.h"
#include "input.h"
#include "actions.h"
#include "types.h"

namespace {

Adafruit_NeoPixel keyLeds(NUMPIXELS_KEYS, PIN_LEDS_KEY, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel baseLeds(NUMPIXELS_BASE, PIN_LEDS_BASE, NEO_GRB + NEO_KHZ800);

uint32_t shown_[NUM_KEYS];
uint32_t lastShow_ = 0;
bool     first_ = true;

uint32_t colorFor(uint8_t i) {
  const KeyConfig &k = g_cfg.keys[i];
  if (input::isDown(i)) return k.color_press;
  if (k.mode == MODE_TOGGLE || k.mode == MODE_AFK)
    return actions::isOn(i) ? k.color_on : k.color_off;
  return k.color_idle;
}

} // namespace

namespace leds {

void begin() {
  keyLeds.begin();
  baseLeds.begin();
  keyLeds.setBrightness(g_cfg.brightness);
  baseLeds.setBrightness(g_cfg.brightness);
  keyLeds.clear();  keyLeds.show();
  baseLeds.fill(g_cfg.base_color);
  baseLeds.show();
}

void startupAnimation() {
  for (uint8_t pass = 0; pass < 2; pass++) {
    for (uint8_t n = 0; n < NUMPIXELS_KEYS; n++) {
      uint8_t i = pass == 0 ? n : (NUMPIXELS_KEYS - 1 - n);
      keyLeds.clear();
      keyLeds.setPixelColor(i, 0x960037);
      keyLeds.show();
      delay(60);
    }
  }
  keyLeds.clear();
  keyLeds.show();
}

void update() {
  uint32_t now = millis();
  bool changed = first_;
  uint32_t c[NUM_KEYS];
  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    c[i] = colorFor(i);
    if (c[i] != shown_[i]) changed = true;
  }
  if (!changed || (now - lastShow_) < 5) return;   // show() desactiva interrupciones ~300us

  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    shown_[i] = c[i];
    keyLeds.setPixelColor(i, c[i]);              // LED n = tecla n
  }
  keyLeds.show();
  lastShow_ = now;
  first_ = false;
}

} // namespace leds
