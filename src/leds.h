#pragma once
#include <stdint.h>

namespace leds {
  void begin();
  void startupAnimation();   // bloqueante, solo usar en setup()
  void update();             // calcula colores; show() solo si algo cambió
}
