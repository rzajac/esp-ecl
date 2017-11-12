## esp_gpio 

Provides GPIO pin manipulation macros operating on registers which is 
faster then SDK provided functions.

Before you can use the pin as GPIO you have to call `esp_gpio_setup` function
once. It works very similar to Arduino's `pin_mode` function. 

See library documentation in [esp_gpio.h](include/esp_gpio.h) header file for 
more details.

#### TODO

- [ ] GPIO16 support. 
- [ ] Interrupt support.
