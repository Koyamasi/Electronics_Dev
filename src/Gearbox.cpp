#include "Gearbox.h"

Gearbox::Gearbox()
{
    this->park_button = new Button();
    this->drive_button = new Button();
    this->neutral_button = new Button();
    this->rear_button = new Button();
    this->manual_button = new Button();

    this->park_led = new Led();
    this->drive_led = new Led();
    this->neutral_led = new Led();
    this->rear_led = new Led();
    this->manual_led = new Led();
}

void Gearbox::init(std::vector<Button>& buttons, std::vector<Led>& leds) 
{
    for (auto& b : buttons)
    {
        if(b.get_name().compare("Park_button") == 0) 
        {
            park_button = &b;
        }

        else if(b.get_name().compare("Drive_button") == 0)
        {
            drive_button = &b;
        }
    }
    
    for (auto& l : leds)
    {
        if(l.get_name().compare("Park_led") == 0)
        {
            park_led = &l;
        }
    }

    setGear('P'); // Default to 'P' on startup
}

void Gearbox::update() 
{
    if (park_button->get_state() == HIGH)
    {
        park_button->send_packet();
        park_led->set_state(HIGH);
    }
    else
    {
        park_button->send_packet();
        delay(2000);
        park_led->set_state(HIGH);
    }
    // else if (digitalRead(_rPin) == LOW) setGear('R');
    // else if (digitalRead(_nPin) == LOW) setGear('N');
    // else if (digitalRead(_dPin) == LOW) setGear('D');
    // else if (digitalRead(_mPin) == LOW) setGear('M');
}

void Gearbox::setGear(char gear) 
{
    if (gear != lastGear) {
        delay(500); // Debounce
        setLeds(gear);
        sendPacket(gear);
        lastGear = gear;
    }
}

void Gearbox::setLeds(char gear) 
{
    // digitalWrite(_pled, gear == 'P' ? HIGH : LOW);
    // digitalWrite(_rled, gear == 'R' ? HIGH : LOW);
    // digitalWrite(_nled, gear == 'N' ? HIGH : LOW);
    // digitalWrite(_dled, gear == 'D' ? HIGH : LOW);
    // digitalWrite(_mled, gear == 'M' ? HIGH : LOW);
}

void Gearbox::sendPacket(char gear) 
{
    byte packet[2] = { (byte)gear, '\n' };
    Serial.write(packet, 2);
} 