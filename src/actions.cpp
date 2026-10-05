#include <Arduino.h>
#include "kem_keyboard.h"
#include "actions.h"

#define QN            8
#define TAP_HOLD_MS   10   // tiempo que una tecla se mantiene en un STEP_TAP
#define TAP_GAP_MS    5    // pausa tras soltar, antes del siguiente paso

namespace {

enum Phase : uint8_t { P_NONE = 0, P_TAP_HOLD, P_GAP, P_DELAY };

const Macro *queue_[QN];
uint8_t  qHead_ = 0, qCount_ = 0, qDropped_ = 0;
const Macro *cur_ = nullptr;
uint8_t  idx_ = 0;
uint8_t  phase_ = P_NONE;
uint32_t waitUntil_ = 0;
uint8_t  tapCode_ = 0, tapMod_ = 0;

bool     state_[NUM_KEYS];
uint32_t afkLast_[NUM_KEYS];

void hidDown(uint8_t u) { kbd.down(u); }
void hidUp(uint8_t u)   { kbd.up(u); }
void modsDown(uint8_t m) { for (uint8_t b = 0; b < 8; b++) if (m & (1 << b)) hidDown(0xE0 + b); }
void modsUp(uint8_t m)   { for (uint8_t b = 8; b-- > 0;) if (m & (1 << b)) hidUp(0xE0 + b); }

} // namespace

namespace actions {

void begin() {
  for (uint8_t i = 0; i < NUM_KEYS; i++) { state_[i] = false; afkLast_[i] = 0; }
}

void play(const Macro *m) {
  if (!m || m->len == 0) return;
  if (qCount_ >= QN) { qDropped_++; return; }
  queue_[(qHead_ + qCount_) % QN] = m;
  qCount_++;
}

void handle(const KeyEvent &ev) {
  if (ev.key >= NUM_KEYS) return;
  KeyConfig &k = g_cfg.keys[ev.key];

  if (ev.type == EV_PRESS) {
    switch (k.mode) {
      case MODE_TAP:
        play(&k.on_press);
        break;
      case MODE_TOGGLE:
        state_[ev.key] = !state_[ev.key];
        play(state_[ev.key] ? &k.on_press : &k.on_alt);
        break;
      case MODE_AFK:
        state_[ev.key] = !state_[ev.key];
        if (state_[ev.key]) { play(&k.on_press); afkLast_[ev.key] = millis(); }
        else                { play(&k.on_alt); }
        break;
      default: break;
    }
  } else if (ev.type == EV_RELEASE) {
    if (k.mode == MODE_TAP) play(&k.on_release);
  }
}

void update() {
  uint32_t now = millis();

  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    KeyConfig &k = g_cfg.keys[i];
    if (k.mode == MODE_AFK && state_[i] && k.afk_seconds > 0 &&
        (now - afkLast_[i]) >= (uint32_t)k.afk_seconds * 1000UL) {
      afkLast_[i] = now;
      play(&k.on_press);
    }
  }

  for (;;) {
    if (phase_ != P_NONE) {
      if ((int32_t)(now - waitUntil_) < 0) return;
      if (phase_ == P_TAP_HOLD) {
        hidUp(tapCode_);
        modsUp(tapMod_);
        phase_ = P_GAP;
        waitUntil_ = now + TAP_GAP_MS;
        continue;
      }
      phase_ = P_NONE;
    }

    if (!cur_) {
      if (qCount_ == 0) return;
      cur_ = queue_[qHead_];
      qHead_ = (qHead_ + 1) % QN;
      qCount_--;
      idx_ = 0;
    }
    if (idx_ >= cur_->len) { cur_ = nullptr; continue; }

    const Step &s = cur_->steps[idx_++];
    switch (s.type) {
      case STEP_DOWN:        hidDown(s.code); break;
      case STEP_UP:          hidUp(s.code);   break;
      case STEP_RELEASE_ALL: kbd.releaseAll(); break;
      case STEP_DELAY:
        phase_ = P_DELAY; waitUntil_ = now + s.arg;
        break;
      case STEP_TAP:
        tapCode_ = s.code; tapMod_ = (uint8_t)s.arg;
        modsDown(tapMod_);
        hidDown(tapCode_);
        phase_ = P_TAP_HOLD; waitUntil_ = now + TAP_HOLD_MS;
        break;
      default: cur_ = nullptr; break;   // STEP_END o tipo desconocido
    }
  }
}

bool isOn(uint8_t key) { return key < NUM_KEYS && state_[key]; }
bool busy() { return cur_ != nullptr || qCount_ > 0; }
uint8_t dropped() { return qDropped_; }

} // namespace actions
