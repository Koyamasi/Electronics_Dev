#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <string>
class Button {
public:
    Button();
    //packet content is the name of the button
    Button(uint8_t pin, std::string packet_content);
    void init();
    void update();

    uint8_t get_button_pin();

    void set_state(bool new_state);
    bool get_state();

    void send_packet();
    std::string get_name();
private:
    bool state = HIGH;
    uint8_t button_pin;
    std::string packet_content;
};

#endif 