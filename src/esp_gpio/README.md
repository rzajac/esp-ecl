## esp_gpio 

Provides GPIO pin manipulation macros operating on registers which is 
faster then SDK provided functions.

Before you can use the pin as GPIO you have to call `esp_gpio_setup` function
once. It works very similar to Arduino's `pinMode` function. 

See library documentation in [esp_gpio.h](include/esp_gpio.h) header file and 
example programs for more details.

- [Blink](../../examples/blink)
- [Conditional blink](../../examples/blink_cond)

#### TODO

- [ ] GPIO16 support. 
- [ ] Interrupt support.
