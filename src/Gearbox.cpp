#include "Gearbox.h"

// ---------------------------------------------------------------------------
//  Construction and initialisation
// ---------------------------------------------------------------------------

Gearbox::Gearbox()
    : park_button(nullptr),
      drive_button(nullptr),
      neutral_button(nullptr),
      rear_button(nullptr),
      manual_button(nullptr),
      park_led(nullptr),
      drive_led(nullptr),
      neutral_led(nullptr),
      rear_led(nullptr),
      manual_led(nullptr) {}

void Gearbox::init(std::vector<Button>& buttons, std::vector<Led>& leds)
{
    // Identify relevant buttons by name
    for (auto& b : buttons) {
        const std::string& name = b.get_name();
        if (name == "Park_button") {
            park_button = &b;
        } else if (name == "Drive_button") {
            drive_button = &b;
        } else if (name == "Neutral_button") {
            neutral_button = &b;
        } else if (name == "Rear_button") {
            rear_button = &b;
        } else if (name == "Manual_button") {
            manual_button = &b;
        }
    }

    // Identify LEDs by name
    for (auto& l : leds) {
        const std::string& name = l.get_name();
        if (name == "Park_led") {
            park_led = &l;
        } else if (name == "Drive_led") {
            drive_led = &l;
        } else if (name == "Neutral_led") {
            neutral_led = &l;
        } else if (name == "Rear_led") {
            rear_led = &l;
        } else if (name == "Manual_led") {
            manual_led = &l;
        }
    }

    setGear('P'); // Default to 'P' on startup
}

// ---------------------------------------------------------------------------
//  Main update loop
// ---------------------------------------------------------------------------

void Gearbox::update()
{
    // Refresh the hardware state of all buttons
    if (park_button) park_button->update();
    if (drive_button) drive_button->update();
    if (neutral_button) neutral_button->update();
    if (rear_button) rear_button->update();
    if (manual_button) manual_button->update();

    // Buttons use INPUT_PULLUP, so pressed == LOW
    if (park_button && park_button->get_state() == LOW) {
        setGear('P');
    } else if (rear_button && rear_button->get_state() == LOW) {
        setGear('R');
    } else if (neutral_button && neutral_button->get_state() == LOW) {
        setGear('N');
    } else if (drive_button && drive_button->get_state() == LOW) {
        setGear('D');
    } else if (manual_button && manual_button->get_state() == LOW) {
        setGear('M');
    }
}

// ---------------------------------------------------------------------------
//  Helpers
// ---------------------------------------------------------------------------

void Gearbox::setGear(char gear)
{
    if (gear != lastGear) {
        delay(500); // Debounce mechanical shift levers
        setLeds(gear);
        sendPacket(gear);
        lastGear = gear;
    }
}

void Gearbox::setLeds(char gear)
{
    if (park_led) {
        park_led->set_state(gear == 'P' ? HIGH : LOW);
        park_led->update();
    }
    if (rear_led) {
        rear_led->set_state(gear == 'R' ? HIGH : LOW);
        rear_led->update();
    }
    if (neutral_led) {
        neutral_led->set_state(gear == 'N' ? HIGH : LOW);
        neutral_led->update();
    }
    if (drive_led) {
        drive_led->set_state(gear == 'D' ? HIGH : LOW);
        drive_led->update();
    }
    if (manual_led) {
        manual_led->set_state(gear == 'M' ? HIGH : LOW);
        manual_led->update();
    }
}

void Gearbox::sendPacket(char gear)
{
    byte packet[2] = { (byte)gear, '\n' };
    Serial.write(packet, 2);
}


