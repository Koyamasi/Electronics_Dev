#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <string>

class Led {
public:
    
    Led(uint8_t pin, std::string name);
    void init();
    void update();

    void toggleLed();

private:
    bool state = LOW;
    uint8_t led_pin;
    std::string name;
};

#endif 