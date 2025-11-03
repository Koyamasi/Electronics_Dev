#ifndef Gearbox_analog_H
#define Gearbox_analog_H

#include <Arduino.h>
#include "Button.h"
#include "Led.h"

#include <string>
#include <vector>

class Gearbox_analog {
public:
    Gearbox_analog();

    int analog_pin;
    int range;

    struct MidPoint {
        String name;
        int value;
        char gear; // 'P','R','N','D','M'
    };

    // Named midpoints (kept in file order)
    std::vector<MidPoint> mid_points;

    // Pointers to indicator LEDs for each gear.
    Led* park_led;
    Led* drive_led;
    Led* neutral_led;
    Led* rear_led;
    Led* manual_led;

    // Keep your existing signature
    void init(std::vector<Led>& leds);

    // Legacy initializer (kept for compatibility).
    void init_midpoints_and_ranges(std::vector<int> parsed_points, int parsed_ranges);

    // Preferred: load config from file (SPIFFS/LittleFS). Default path matches your spec.
    void init_midpoints_and_ranges_from_file(const char* path = "/config_analog_gear.txt");

    void update();

private:
    void setGear(char gear);
    void setLeds(char gear);
    void sendPacket(char gear);

    static char inferGearFromName(const String& name);
    static String trimCopy(const String& s);

    char lastGear = '\0';
};

#endif
