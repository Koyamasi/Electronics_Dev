#ifndef GEARBOX_H
#define GEARBOX_H

#include <Arduino.h>
#include "Button.h"
#include "Led.h"

#include <string>
#include <vector>

class Gearbox {
public:
    Gearbox();

    // Pointers to the button objects controlling each gear.
    Button* park_button;
    Button* drive_button;
    Button* neutral_button;
    Button* rear_button;
    Button* manual_button;

    // Pointers to indicator LEDs for each gear.
    Led* park_led;
    Led* drive_led;
    Led* neutral_led;
    Led* rear_led;
    Led* manual_led;

    void init(std::vector<Button>& buttons, std::vector<Led>& leds);
    void update();

private:
    void setGear(char gear);
    void setLeds(char gear);
    void sendPacket(char gear);

    char lastGear = '\0';
};

#endif

