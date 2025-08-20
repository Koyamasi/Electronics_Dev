#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <string>

class Led {
public:
    // name is identifier for this LED
    Led(uint8_t pin, std::string name);
    void init();
    void update();

    uint8_t get_led_pin();

    void set_state(bool new_state);
    bool get_state();

    void toggleLed();

    void send_packet();

private:
    bool state = LOW;
    uint8_t led_pin;
    std::string name;
};

#endif 