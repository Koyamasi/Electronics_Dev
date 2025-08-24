#ifndef GEARBOX_H
#define GEARBOX_H

#include <Arduino.h>
#include "Button.h"
#include "Led.h"

#include <string>
#include <vector>

/**
 * @brief Implements a simple automatic gearbox state machine.
 *
 * Gearbox observes dedicated buttons for each gear position and updates
 * indicator LEDs and serial output whenever the selected gear changes.
 */
class Gearbox {
public:
    Gearbox();

    // Pointers to the button objects controlling each gear.
    Button* park_button;
    Button* drive_button;
    Button* neutral_button;
    Button* rear_button;
    Button* manual_button;

    // Pointers to indicator LEDs for each gear.
    Led* park_led;
    Led* drive_led;
    Led* neutral_led;
    Led* rear_led;
    Led* manual_led;

    /**
     * @brief Locate the relevant buttons/LEDs from the provided lists.
     */
    void init(std::vector<Button>& buttons, std::vector<Led>& leds);

    /// Poll inputs and update LEDs and serial output.
    void update();

private:
    void setGear(char gear);      ///< Handle gear change
    void setLeds(char gear);      ///< Update indicator LEDs
    void sendPacket(char gear);   ///< Send selected gear over Serial

    char lastGear = '\0';        ///< Previously selected gear
};

#endif // GEARBOX_H


