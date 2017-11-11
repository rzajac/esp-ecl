## ESP8266 Common Libraries (ECL)

Repository is a collection of small but useful libraries I use in many of my 
projects gathered in one place. Each of the libraries has very well documented 
header file. Below is the list of available libraries:
 
Library name                                   | Description
-----------------------------------------------|-------------
[esp_aes](src/include/esp_aes.h)               | AES CBC encryption / decryption.
[esp_cfg](src/include/esp_cfg.h)               | Read / write custom configuration structures from / to flash.
[esp_eb](src/include/esp_eb.h)                 | Simple event bus.
[esp_gpio](src/include/esp_gpio.h)             | Fast GPIO manipulation library.
[esp_gpio_debug](src/include/esp_gpio_debug.h) | GPIO debugging library.
[esp_json](src/include/esp_json.h)             | Easier JSON manipulation.
[esp_sdo](src/include/esp_sdo.h)               | Use RX pin as the GPIO3.
[esp_tim](src/include/esp_tim.h)               | Collection of helpers to deal with timers and delays.
[esp_util](src/include/esp_util.h)             | Utility functions.

## Build environment.

This library is part of my build system for ESP8266 based on CMake.
To compile / flash examples you will have to have the ESP development 
environment setup as described at https://github.com/rzajac/esp-dev-env.

## Examples.

Libraries come with few examples to show how to use them.

- [AES Encrypt/Decrypt](examples/aes)
- [Blink LED](examples/blink)
- [Conditional LED blink](examples/blink_cond)
- [Custom config](examples/cfg)
- [Evens](examples/events)
- [JSON](examples/json)
- [Timer](examples/timer)

## Integration.

If you're using my build environment you can install this library by issuing:

```
$ wget -O - https://raw.githubusercontent.com/rzajac/esp-ecl/master/install.sh | bash
```

or if you already cloned this repository you can do:

```
$ cd build
$ cmake ..
$ make
$ make install
```

which will install the library, headers and scripts in appropriate places 
in `$ESPROOT`.

## License.

[Apache License Version 2.0](LICENSE) unless stated otherwise.
