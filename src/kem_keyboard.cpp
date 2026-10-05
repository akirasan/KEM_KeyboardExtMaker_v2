#include <Arduino.h>
#include <string.h>
#include "kem_hid.h"
#include "kem_keyboard.h"

// Descriptor de teclado boot-compatible, Report ID 2 (idéntico al de la librería Arduino Keyboard)
static const uint8_t kbdDescriptor[] = {
  0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
  0x09, 0x06,        // USAGE (Keyboard)
  0xa1, 0x01,        // COLLECTION (Application)
  0x85, 0x02,        //   REPORT_ID (2)
  0x05, 0x07,        //   USAGE_PAGE (Keyboard)
  0x19, 0xe0,        //   USAGE_MINIMUM (LeftControl)
  0x29, 0xe7,        //   USAGE_MAXIMUM (Right GUI)
  0x15, 0x00,        //   LOGICAL_MINIMUM (0)
  0x25, 0x01,        //   LOGICAL_MAXIMUM (1)
  0x75, 0x01,        //   REPORT_SIZE (1)
  0x95, 0x08,        //   REPORT_COUNT (8)
  0x81, 0x02,        //   INPUT (Data,Var,Abs)   modificadores
  0x95, 0x01,        //   REPORT_COUNT (1)
  0x75, 0x08,        //   REPORT_SIZE (8)
  0x81, 0x03,        //   INPUT (Cnst,Var,Abs)   reservado
  0x95, 0x06,        //   REPORT_COUNT (6)
  0x75, 0x08,        //   REPORT_SIZE (8)
  0x15, 0x00,        //   LOGICAL_MINIMUM (0)
  0x25, 0x73,        //   LOGICAL_MAXIMUM (115)
  0x05, 0x07,        //   USAGE_PAGE (Keyboard)
  0x19, 0x00,        //   USAGE_MINIMUM (0)
  0x29, 0x73,        //   USAGE_MAXIMUM (115 = F24)
  0x81, 0x00,        //   INPUT (Data,Ary,Abs)   6 teclas
  0xc0               // END_COLLECTION
};

KemKeyboard kbd;

void KemKeyboard::begin() {
  static KHIDSubDescriptor node(kbdDescriptor, sizeof(kbdDescriptor));
  static bool registered = false;
  if (!registered) {            // el descriptor debe añadirse antes de la enumeración USB
    KHID().AppendDescriptor(&node);
    registered = true;
  }
  memset(&rep_, 0, sizeof(rep_));
}

void KemKeyboard::send() {
  KHID().SendReport(2, &rep_, sizeof(rep_));
}

bool KemKeyboard::down(uint8_t u) {
  if (u >= 0xE0 && u <= 0xE7) {
    rep_.modifiers |= (uint8_t)(1 << (u - 0xE0));
  } else if (u >= 0x04 && u <= 0x73) {
    int slot = -1;
    for (uint8_t i = 0; i < 6; i++) {
      if (rep_.keys[i] == u) { slot = -2; break; }      // ya pulsada
      if (rep_.keys[i] == 0 && slot == -1) slot = i;
    }
    if (slot == -1) return false;                       // 6 teclas ocupadas
    if (slot >= 0) rep_.keys[slot] = u;
  } else {
    return false;
  }
  send();
  return true;
}

bool KemKeyboard::up(uint8_t u) {
  if (u >= 0xE0 && u <= 0xE7) {
    rep_.modifiers &= (uint8_t)~(1 << (u - 0xE0));
  } else if (u >= 0x04 && u <= 0x73) {
    for (uint8_t i = 0; i < 6; i++) if (rep_.keys[i] == u) rep_.keys[i] = 0;
  } else {
    return false;
  }
  send();
  return true;
}

void KemKeyboard::releaseAll() {
  memset(&rep_, 0, sizeof(rep_));
  send();
}
