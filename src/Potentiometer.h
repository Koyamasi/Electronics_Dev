#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <Arduino.h>
#include <string>

class Potentiometer {
public:
    Potentiometer();
    Potentiometer(uint8_t pin, std::string packet_content);
    void init();
    void update();

    uint8_t get_pin();

    void set_state(uint8_t new_state);
    uint8_t get_state();

    void send_packet();
    std::string get_name();

private:
    uint8_t state = 0;
    uint8_t pot_pin;
    std::string packet_content;
};

#endif
