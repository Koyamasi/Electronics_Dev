#include "Button.h"

Button::Button(uint8_t pin, std::string packet_content) 
{
    this->button_pin = pin;
    this->packet_content = packet_content;
}

bool Button::get_state()
{
    return this->state;
}

void Button::set_state(bool new_state)
{
    this->state = new_state;
}

uint8_t Button::get_button_pin()
{
    return this->button_pin;
}

void Button::init()
{
    pinMode(this->button_pin, INPUT_PULLUP);
    //every button upon initialising software, reads and initialises the correct state.
    this->set_state(digitalRead(this->button_pin));
    delay (4000);
    this->send_packet();
}

void Button::update()
{
    bool current_state = digitalRead(this->button_pin);
    if (current_state != this->state) {
        this->set_state(current_state);
        this->send_packet();
    }
}

void Button::send_packet() {
    const char stateCh = (this->state == LOW) ? 'N' : 'F'; // adjust if your logic differs
    constexpr uint8_t SEP = 0x1F; // ASCII Unit Separator

    // Write: <packet_content><US><N_or_F>\n
    Serial.write(reinterpret_cast<const uint8_t*>(packet_content.data()),
                 packet_content.size());
    Serial.write(SEP);
    Serial.write(static_cast<uint8_t>(stateCh));
    Serial.write('\n'); // record terminator
}
