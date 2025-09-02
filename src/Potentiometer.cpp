#include "Potentiometer.h"

static constexpr int NUM_STATES = 6;
static constexpr int ADC_MAX = 4095;

Potentiometer::Potentiometer()
{
    ;
}

Potentiometer::Potentiometer(uint8_t pin, std::string packet_content)
{
    this->pot_pin = pin;
    this->packet_content = packet_content;
}

uint8_t Potentiometer::get_pin()
{
    return this->pot_pin;
}

void Potentiometer::set_state(uint8_t new_state)
{
    this->state = new_state;
}

uint8_t Potentiometer::get_state()
{
    return this->state;
}

void Potentiometer::init()
{
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
    analogReadResolution(12);
#endif
    pinMode(this->pot_pin, INPUT);
    int raw = analogRead(this->pot_pin);
    uint8_t new_state = raw / ((ADC_MAX + 1) / NUM_STATES);
    if (new_state >= NUM_STATES) new_state = NUM_STATES - 1;
    this->set_state(new_state);
}

void Potentiometer::update()
{
    int raw = analogRead(this->pot_pin);
    uint8_t new_state = raw / ((ADC_MAX + 1) / NUM_STATES);
    if (new_state >= NUM_STATES) new_state = NUM_STATES - 1;
    if (new_state != this->state)
    {
        this->set_state(new_state);
        this->send_packet();
    }
}

void Potentiometer::send_packet()
{
    char stateCh = '0' + this->state;
    constexpr uint8_t SEP = 0x1F;

    Serial.write(reinterpret_cast<const uint8_t*>(packet_content.data()),
                 packet_content.size());
    Serial.write(SEP);
    Serial.write(static_cast<uint8_t>(stateCh));
    Serial.write('\n');
}

std::string Potentiometer::get_name()
{
    return this->packet_content;
}

