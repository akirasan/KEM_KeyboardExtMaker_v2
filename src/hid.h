#pragma once
#include <stdint.h>

// Códigos HID Usage (Keyboard/Keypad page 0x07), independientes del idioma del SO.
// Modificadores: máscara de bits (bit n -> usage 0xE0+n)
#define MOD_LCTRL  0x01
#define MOD_LSHIFT 0x02
#define MOD_LALT   0x04
#define MOD_LGUI   0x08
#define MOD_RCTRL  0x10
#define MOD_RSHIFT 0x20
#define MOD_RALT   0x40  // AltGr
#define MOD_RGUI   0x80

#define HID_A          0x04   // A..Z = 0x04..0x1D
#define HID_1          0x1E   // 1..9 = 0x1E..0x26
#define HID_0          0x27
#define HID_ENTER      0x28
#define HID_ESC        0x29
#define HID_BACKSPACE  0x2A
#define HID_TAB        0x2B
#define HID_SPACE      0x2C
#define HID_F1         0x3A   // F1..F12 = 0x3A..0x45
#define HID_PRINTSCR   0x46
#define HID_DELETE     0x4C
#define HID_RIGHT      0x4F
#define HID_LEFT       0x50
#define HID_DOWN       0x51
#define HID_UP         0x52
#define HID_F13        0x68   // F13..F24 = 0x68..0x73

#define HID_LETTER(c)  (HID_A + ((c) - 'a'))
#define HID_FKEY(n)    ((n) <= 12 ? HID_F1 + (n) - 1 : HID_F13 + (n) - 13)
