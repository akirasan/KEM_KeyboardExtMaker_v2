#include <Arduino.h>
#include "input.h"
#include "pins.h"

#define QSIZE 32

namespace {

enum AdcClass : uint8_t { ADC_IDLE = 0, ADC_K7 = 7, ADC_K8 = 8 };

KeyEvent queue_[QSIZE];
uint8_t  qHead_ = 0, qCount_ = 0, qDropped_ = 0;

bool     down_[NUM_KEYS];       // estado estable (tras debounce)
uint16_t cnt_[NUM_KEYS];        // ms que el estado crudo lleva distinto del estable
uint32_t downAt_[NUM_KEYS];
bool     longFired_[NUM_KEYS];

uint32_t lastMs_ = 0;
uint8_t  adcCls_ = ADC_IDLE;
uint32_t lastK7_ = 0;

void push(uint8_t key, uint8_t type) {
  if (qCount_ >= QSIZE) { qDropped_++; return; }
  KeyEvent &e = queue_[(qHead_ + qCount_) % QSIZE];
  e.key = key; e.type = type;
  qCount_++;
}

uint16_t readAdcAvg() {
  uint32_t s = 0;
  for (uint8_t i = 0; i < ADC_SAMPLES; i++) s += analogRead(PIN_KEY78);
  return s / ADC_SAMPLES;
}

// Clasifica la lectura de D7 con histéresis. Las zonas ambiguas conservan la clase previa.
void classify(uint16_t v) {
  if (v < ADC_K7_ON)                              adcCls_ = ADC_K7;
  else if (adcCls_ == ADC_K7 && v < ADC_K7_OFF)   adcCls_ = ADC_K7;
  else if (v >= ADC_K8_MIN && v <= ADC_K8_MAX)    adcCls_ = ADC_K8;
  else if (v >= ADC_IDLE_MIN)                     adcCls_ = ADC_IDLE;
}

void debounce(uint8_t k, bool raw, uint32_t now, uint16_t dt) {
  if (raw == down_[k]) { cnt_[k] = 0; return; }
  cnt_[k] += dt;
  uint16_t need = raw ? DEBOUNCE_PRESS_MS : DEBOUNCE_RELEASE_MS;
  if (cnt_[k] < need) return;

  cnt_[k] = 0;
  down_[k] = raw;
  if (raw) {
    downAt_[k] = now;
    longFired_[k] = false;
    push(k, EV_PRESS);
  } else {
    push(k, EV_RELEASE);
    if (!longFired_[k]) push(k, EV_TAP);
  }
}

} // namespace

namespace input {

void begin() {
  for (uint8_t i = 0; i < 6; i++) pinMode(KEY_PINS_DIGITAL[i], INPUT_PULLUP);
  pinMode(PIN_KEY78, INPUT_PULLUP);
  analogReadResolution(12);
  for (uint8_t i = 0; i < NUM_KEYS; i++) { down_[i] = false; cnt_[i] = 0; longFired_[i] = false; }
  lastMs_ = millis();
  lastK7_ = lastMs_;
}

void update() {
  uint32_t now = millis();
  uint32_t d = now - lastMs_;
  if (d == 0) return;                       // escaneo a 1 kHz
  lastMs_ = now;
  uint16_t dt = d > 50 ? 50 : (uint16_t)d;

  for (uint8_t i = 0; i < 6; i++)
    debounce(i, digitalRead(KEY_PINS_DIGITAL[i]) == LOW, now, dt);

  classify(readAdcAvg());
  if (adcCls_ == ADC_K7) lastK7_ = now;
  bool k8ok = (now - lastK7_) >= K8_LOCKOUT_MS;
  debounce(6, adcCls_ == ADC_K7, now, dt);
  debounce(7, adcCls_ == ADC_K8 && k8ok, now, dt);

  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    if (down_[i] && !longFired_[i] && (now - downAt_[i]) >= LONG_PRESS_MS) {
      longFired_[i] = true;
      push(i, EV_LONG);
    }
  }
}

bool poll(KeyEvent &ev) {
  if (qCount_ == 0) return false;
  ev = queue_[qHead_];
  qHead_ = (qHead_ + 1) % QSIZE;
  qCount_--;
  return true;
}

bool isDown(uint8_t key) { return key < NUM_KEYS && down_[key]; }
uint8_t dropped() { return qDropped_; }

} // namespace input
