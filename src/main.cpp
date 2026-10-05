#include <Arduino.h>
#include "kem_keyboard.h"
#include "config.h"
#include "input.h"
#include "actions.h"
#include "leds.h"

#ifdef KEM_DEBUG
static const char *EV_NAMES[] = {"PRESS", "RELEASE", "TAP", "LONG"};
static uint32_t loopMaxUs = 0, lastStat = 0;
#endif

void setup() {
#ifdef KEM_DEBUG
  Serial.begin(115200);
#endif
  config::loadDefaults();
  kbd.begin();
  leds::begin();
  leds::startupAnimation();
  input::begin();
  actions::begin();
}

void loop() {
#ifdef KEM_DEBUG
  uint32_t t0 = micros();
#endif

  input::update();

  KeyEvent ev;
  while (input::poll(ev)) {
    actions::handle(ev);
#ifdef KEM_DEBUG
    if (Serial) {
      Serial.print("K"); Serial.print(ev.key + 1);
      Serial.print(" "); Serial.print(EV_NAMES[ev.type]);
      Serial.print("  t="); Serial.println(millis());
    }
#endif
  }

  actions::update();
  leds::update();

#ifdef KEM_DEBUG
  uint32_t dt = micros() - t0;
  if (dt > loopMaxUs) loopMaxUs = dt;
  if (millis() - lastStat >= 5000) {
    lastStat = millis();
    if (Serial) {
      Serial.print("[stat] loop max us="); Serial.print(loopMaxUs);
      Serial.print(" dropIn="); Serial.print(input::dropped());
      Serial.print(" dropAct="); Serial.println(actions::dropped());
    }
    loopMaxUs = 0;
  }
#endif
}
