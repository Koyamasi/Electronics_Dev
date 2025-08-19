#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>   // change to <SPIFFS.h> if you prefer SPIFFS
#include <vector>
#include <string>
#include <istream>
#include "Button.h"

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

    InputSystem();                  // defaults to "/config.txt"
    bool begin();                   // mounts LittleFS, loads & parses file
    void update();                  // high-level update
    void buttons_update();          // updates all buttons
    static void printConfig(const ConfigData& cfg);

    const std::vector<Button>& getButtons() const { return buttons; }

private:
    ConfigData parseConfig(std::istream& in);
    void initButtonsFromConfig(const ConfigData& cfg);

    String fsPath;                  // "/config.txt"
    std::vector<Button> buttons;    // Buttons + DpadButtons
};

#endif // INPUTSYSTEM_H
