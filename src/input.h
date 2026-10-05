#pragma once
#include <stdint.h>

enum EventType : uint8_t {
  EV_PRESS = 0,   // flanco de bajada (debounced)
  EV_RELEASE,     // flanco de subida (debounced)
  EV_TAP,         // soltada antes de LONG_PRESS_MS (se emite tras RELEASE)
  EV_LONG         // se emite una vez al superar LONG_PRESS_MS mientras sigue pulsada
};

struct KeyEvent {
  uint8_t key;    // 0..7
  uint8_t type;   // EventType
};

#define DEBOUNCE_PRESS_MS    4
#define DEBOUNCE_RELEASE_MS  8
#define LONG_PRESS_MS        600

namespace input {
  void begin();
  void update();                 // llamar en cada loop(); escanea 1 vez por ms
  bool poll(KeyEvent &ev);       // saca el siguiente evento de la cola
  bool isDown(uint8_t key);
  uint8_t dropped();             // eventos perdidos por cola llena
}
