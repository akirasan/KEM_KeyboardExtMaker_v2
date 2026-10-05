#pragma once
#include <stdint.h>

// Teclado HID propio (no usa la librería Keyboard). Trabaja con códigos HID Usage:
//   0xE0..0xE7 modificadores, 0x04..0x73 teclas (hasta F24).
class KemKeyboard {
public:
  void begin();
  bool down(uint8_t usage);
  bool up(uint8_t usage);
  void releaseAll();
private:
  struct Report { uint8_t modifiers; uint8_t reserved; uint8_t keys[6]; };
  Report rep_;
  void send();
};

extern KemKeyboard kbd;
