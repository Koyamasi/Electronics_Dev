#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <string>

/**
 * @brief Represents a digital button connected to the controller.
 *
 * The Button class tracks state changes on a GPIO pin configured with
 * ``INPUT_PULLUP``. Whenever the state toggles, a packet containing the
 * button name and its new state is sent over the serial connection.
 */
class Button {
public:
    Button();

    /**
     * @brief Construct a new Button instance.
     * @param pin GPIO pin for the button.
     * @param packet_content Name to transmit when the button changes state.
     */
    Button(uint8_t pin, std::string packet_content);

    /// Configure the pin mode and read the initial state.
    void init();

    /// Poll the button and send a packet if the state changed.
    void update();

    /// @return The GPIO pin associated with the button.
    uint8_t get_button_pin();

    /// Cache a new button state.
    void set_state(bool new_state);

    /// @return The last observed button state (LOW if pressed).
    bool get_state();

    /// Transmit the current button state over Serial.
    void send_packet();

    /// @return The textual name used in packets.
    std::string get_name();

private:
    bool state = HIGH;          ///< HIGH means not pressed due to pull-up
    uint8_t button_pin;         ///< GPIO pin number
    std::string packet_content; ///< Identifier sent in packets
};

#endif // BUTTON_H

