#include "Button.h"

// ---------------------------------------------------------------------------
//  Construction and configuration
// ---------------------------------------------------------------------------

Button::Button() {}

Button::Button(uint8_t pin, std::string packet_content)
    : button_pin(pin), packet_content(std::move(packet_content)) {}

// ---------------------------------------------------------------------------
//  Accessors
// ---------------------------------------------------------------------------

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

std::string Button::get_name()
{
    return this->packet_content;
}

// ---------------------------------------------------------------------------
//  Behaviour
// ---------------------------------------------------------------------------

void Button::init()
{
    pinMode(this->button_pin, INPUT_PULLUP);
    // Read the hardware state at startup so the software matches the button.
    this->set_state(digitalRead(this->button_pin));
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
    const char stateCh = (this->state == LOW) ? 'U' : 'D'; // U = up, D = down
    constexpr uint8_t SEP = 0x1F; // ASCII Unit Separator

    // Write: <packet_content><US><U_or_D>\n
    Serial.write(reinterpret_cast<const uint8_t*>(packet_content.data()),
                 packet_content.size());
    Serial.write(SEP);
    Serial.write(static_cast<uint8_t>(stateCh));
    Serial.write('\n'); // record terminator
}

