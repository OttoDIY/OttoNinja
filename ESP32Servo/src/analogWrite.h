#if !defined (ESP32AnalogWrite)
#define ESP32AnalogWrite
#include <cstdint>
#include <esp32-hal-ledc.h>
#include <Arduino.h>

  void analogWrite( uint8_t APin, uint16_t AValue );

#endif
