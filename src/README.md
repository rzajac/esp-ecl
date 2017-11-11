## esp_aes

Adoption of https://github.com/kokke/tiny-AES-c library for ESP8266.
Comes with [gen_aes.sh](../bin/gen_aes.sh) helper script to generate 
cryptographic keys using `openssl rand`. See library documentation in 
[esp_aes.h](include/esp_aes.h) header file for more details.

## esp_cfg

Library for storing custom data structures on flash to keep it between 
resets / reboots. Depending on your configuration, linker script and 
data size you may have to customize `ESP_CFG_START_SECTOR` in `esp_config.h` 
file. By default it's set to sector `0xC` (one sector is 4096 bytes) which 
is located before user app (`0x10000`).  

By default user can use two configuration structures. You may change it by 
defining `ESP_CFG_NUMBER` or editing `esp_config.h` file (making this 
change requires changing `ESP_CFG_START_SECTOR`). See its documentation 
in [esp_cfg.h](include/esp_cfg.h) header file for more details.

# esp_eb

In event driven environment like ESP8266 event bus helps make your code less
"linear". Especially when you have to make your methods very fast. 

![EventBus](../doc/EventBus.png)

With this library you will be able to define custom events and react to them. 
You will be able to attach / detach your functions to handle your events.
The additional feature is that all event callbacks are scheduled with timers 
not to block the CPU when there are many event listeners.
 
This library will allow you to:
 - create custom events
 - trigger custom events
 - attach / detach event listeners (callbacks)
 - pass arguments during event trigger
 
See library documentation in [esp_eb.h](include/esp_eb.h) header file for 
more details.

## esp_gpio 

Provides GPIO pin manipulation macros operating on registers which is 
faster then SDK provided functions.

Before you can use the pin as GPIO you have to call `esp_gpio_setup` function
once. It works very similar to Arduino's `pin_mode` function. See library 
documentation in [esp_gpio.h](include/esp_gpio.h) header file for more details.

#### TODO

- [ ] GPIO16 support. 
- [ ] Interrupt support.

## esp_gpio_debug

Provides set of functions to dump GPIO register values for debugging purposes.
See library documentation in [esp_gpio_debug.h](include/esp_gpio_debug.h) 
header file for more details.

## esp_json 

This library is an adoption of https://github.com/DaveGamble/cJSON for ESP8266 
which makes JSON manipulation easier.
See library documentation in [esp_json.h](include/esp_json.h) header file for 
more details.

## esp_sdo

>Stupid bit of code that does the bare minimum to make os_printf work.
 
Jeroen Domburg `stdout.c` code allowing to use RX pin as the GPIO3. See library 
documentation in [esp_sdo.h](include/esp_sdo.h) header file for more details.

## esp_tim

Library is a thin wrapper around SDK provided timer functions and makes  
starting, stopping and disarming timers with payloads easier.   
See library documentation in [esp_tim.h](include/esp_tim.h) header file for 
more details.

## esp_util

The `esp_util` is collection of helper functions which do not belong anywhere 
else. See library documentation in [esp_util.h](include/esp_util.h) header file 
for more details.
