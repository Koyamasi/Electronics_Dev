#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <Arduino.h>
#include <vector>
#include <string>
#include <istream>

#include <FS.h>
#include <LittleFS.h>

#include "Button.h"
#include "Potentiometer.h"
#include "Led.h"
#include "Gearbox_analog.h"

class InputSystem {
public:
  struct ConfigEntry {
    std::string name;
    int value;
    ConfigEntry(const std::string& n, int v);
  };

  struct ConfigData {
    std::vector<ConfigEntry> buttons;
    std::vector<ConfigEntry> potentiometers;
    std::vector<ConfigEntry> dpadButtons;
    std::vector<ConfigEntry> leds;
  };

  InputSystem();

  bool begin();
  void update();

  // optional for debugging
  static void printConfig(const ConfigData& cfg);
  static ConfigData parseConfig(std::istream& in);

private:
  void buttons_update();
  void potentiometers_update();
  void update_led();

  void initButtonsFromConfig(const ConfigData& cfg);
  void initLedsFromConfig(const ConfigData& cfg);
  void initPotentiometersFromConfig(const ConfigData& cfg);

private:
  std::string fsPath;

  std::vector<Button> buttons;
  std::vector<Potentiometer> potentiometers;
  std::vector<Led> leds;

public:
  // expose if other subsystems need it
  Gearbox_analog gearbox_analog;
};

#endif
