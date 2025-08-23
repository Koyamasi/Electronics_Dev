#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>   // change to <SPIFFS.h> if you prefer SPIFFS
#include <vector>
#include <string>
#include <istream>
#include "Button.h"
#include "Led.h"
#include "Gearbox.h"

/**
 * @brief High-level manager for button and LED peripherals.
 *
 * InputSystem loads configuration data from the filesystem, creates the
 * requested Button and Led objects and periodically updates them. It also
 * delegates gearbox-specific behaviour to the Gearbox helper class.
 */
class InputSystem {
public:
    struct ConfigEntry {
        std::string name; ///< Identifier in the config file
        int value;        ///< Numeric value (e.g. pin number)
        ConfigEntry(const std::string& n, int v);
    };

    struct ConfigData {
        std::vector<ConfigEntry> buttons;
        std::vector<ConfigEntry> potentiometers;
        std::vector<ConfigEntry> dpadButtons;
        std::vector<ConfigEntry> leds;
    };

    InputSystem();                  ///< defaults to "/config.txt"
    bool begin();                   ///< mounts LittleFS, loads & parses file
    void update();                  ///< high-level update
    void buttons_update();          ///< updates all buttons
    void update_led();              ///< updates all leds
    static void printConfig(const ConfigData& cfg);

    const std::vector<Button>& getButtons() const { return buttons; }

private:
    ConfigData parseConfig(std::istream& in);
    void initButtonsFromConfig(const ConfigData& cfg);
    void initLedsFromConfig(const ConfigData& cfg);

    String fsPath;                  ///< path to configuration file
    std::vector<Button> buttons;    ///< Buttons + DpadButtons
    std::vector<Led> leds;          ///< LED outputs
    Gearbox gearbox;                ///< Gearbox controller
};

#endif // INPUTSYSTEM_H
