#pragma once
#include <stdint.h>

// Hardware PCB KEM (XIAO SAMD21) - no modificar sin cambiar la PCB
#define PIN_LEDS_KEY    10
#define PIN_LEDS_BASE   9
#define NUMPIXELS_KEYS  8
#define NUMPIXELS_BASE  4

#define NUM_KEYS        8

// KEY1..KEY6: digitales (pulsada = LOW, INPUT_PULLUP)
static const uint8_t KEY_PINS_DIGITAL[6] = {0, 1, 2, 3, 6, 8};

// KEY7 y KEY8 comparten D7 (lectura analógica, divisor resistivo)
#define PIN_KEY78       7

// Bandas ADC en 12 bit, medidas en la PCB real (ver test ADC):
//   reposo ~4095 | KEY8 ~2282 | KEY7 ~165 (KEY7 domina si están las dos)
#define ADC_K7_ON       1000   // < esto  -> KEY7 pulsada
#define ADC_K7_OFF      1500   // KEY7 se suelta al superar esto (histéresis)
#define ADC_K8_MIN      1500   // banda KEY8
#define ADC_K8_MAX      3300
#define ADC_IDLE_MIN    3500   // >= esto -> reposo
#define ADC_SAMPLES     8      // promedio por ms
#define K8_LOCKOUT_MS   20     // tras soltar KEY7, ignorar KEY8 (transitorio 150->4095)
