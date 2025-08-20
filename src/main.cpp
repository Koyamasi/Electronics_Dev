// main.cpp
#include <Arduino.h>
#include "InputSystem.h"
// #include "Gearbox.h"  // <- if/when you want to re-enable the gearbox

// If you use the gearbox, restore the pins and object below.
// constexpr uint8_t pPin=45, pled=44, rPin=47, rled=46, nPin=49, nled=48, dPin=51, dled=50, mPin=53, mled=52;
// Gearbox gearbox(pPin, pled, rPin, rled, nPin, nled, dPin, dled, mPin, mled);

static InputSystem inputs;  // parses data/config.txt inside begin()

void setup() {
  Serial.begin(115200);
  delay(50);

  // Initialize from data/config.txt, create & init all buttons (Buttons + DpadButtons)
  inputs.begin();
  // gearbox.begin(); // uncomment if you wire the gearbox back in
}

void loop() {
  inputs.update();   // poll/debounce/send packets for all configured buttons
  // gearbox.update(); // if used
  delay(10);
}
