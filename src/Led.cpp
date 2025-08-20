#include "Led.h"

Led::Led(uint8_t led_pin, std::string name)
{
    this->led_pin = led_pin;
    this->name = name;
}

bool Led::get_state()
{
    return this->state;
}

void Led::set_state(bool new_state)
{
    this->state = new_state;
}

uint8_t Led::get_led_pin()
{
    return this->led_pin;
}

void Led::init()
{
    pinMode(this->led_pin, OUTPUT);
    digitalWrite(this->led_pin, this->state);
}

void Led::update()
{
    digitalWrite(this->led_pin, this->state);
}

void Led::toggle_led()
{
    this->set_state(!this->state);
    //this->send_packet();
}

void Led::send_packet()
{
    const char stateCh = (this->state == HIGH) ? '1' : '0';
    constexpr uint8_t SEP = 0x1F; // ASCII Unit Separator

    // Write: <name><US><0_or_1>\n
    Serial.write(reinterpret_cast<const uint8_t*>(name.data()),
                 name.size());
    Serial.write(SEP);
    Serial.write(static_cast<uint8_t>(stateCh));
    Serial.write('\n'); // record terminator
}