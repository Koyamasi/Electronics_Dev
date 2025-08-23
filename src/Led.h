#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <string>

/**
 * @brief Simple wrapper around a digital output LED.
 *
 * The Led class controls a GPIO pin configured as ``OUTPUT`` and provides
 * helpers for toggling and reporting state changes.
 */
class Led {
public:
    Led();

    /**
     * @brief Construct a new LED object.
     * @param pin  GPIO pin driving the LED.
     * @param name Identifier used when reporting state changes.
     */
    Led(uint8_t pin, std::string name);

    /// Configure the GPIO pin and write the initial state.
    void init();

    /// Refresh the hardware output to match the cached state.
    void update();

    /// @return The GPIO pin number.
    uint8_t get_led_pin();

    /// Set the cached on/off state without updating the hardware.
    void set_state(bool new_state);

    /// @return Current cached state (HIGH for on).
    bool get_state();

    /// Toggle the LED state and optionally send a packet.
    void toggle_led();

    /// Send the LED state over Serial using the configured name.
    void send_packet();

    /// @return The identifier associated with this LED.
    std::string get_name();

private:
    int state = LOW;       ///< Cached LED state
    uint8_t led_pin;       ///< GPIO pin number
    std::string name;      ///< Name used in packets
};

#endif // LED_H

