#ifndef GEARBOX_H
#define GEARBOX_H

#include <Arduino.h>
#include "Button.h"
#include "Led.h"

#include <vector>
#include <string>

class Gearbox {
public:
    Gearbox();
    Button* park_button;
    Button* drive_button;
    Button* neutral_button;
    Button* rear_button;
    Button* manual_button;

    Led* park_led;
    Led* drive_led;
    Led* neutral_led;
    Led* rear_led;
    Led* manual_led;

    void init(std::vector<Button>& buttons, std::vector<Led>& leds);
    void update();

private:
    // int _pPin, _pled;
    // int _rPin, _rled;
    // int _nPin, _nled;
    // int _dPin, _dled;
    // int _mPin, _mled;

    void setGear(char gear);
    void setLeds(char gear);
    void sendPacket(char gear);
    char lastGear = '\0';
};

#endif 