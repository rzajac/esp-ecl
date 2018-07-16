## ESP8266 Common Libraries (ECL).

Repository is a collection of small but useful libraries I use in many of my 
projects gathered in one place. Each of the libraries has a documentation  
`README.md` file as well as very well documented header file. 
 
Library name                   | Description
-------------------------------|-------------
[esp_aes](src/esp_aes)         | AES CBC encryption / decryption.
[config](src/config)           | Read / write custom configuration structures from / to flash.
[event_bus](src/event_bus)     | Simple event bus.
[gpio_helper](src/gpio_helper) | Fast GPIO manipulation library.
[gpio_debug](src/gpio_debug)   | GPIO debugging library.
[esp_json](src/esp_json)       | Easier JSON manipulation.
[esp_sdo](src/esp_sdo)         | Use RX pin as the GPIO3.
[timers](src/timers)           | Collection of helpers to deal with timers and delays.
[utils](src/utils)             | Utility functions.

## Build environment.

Libraries are part of my build system for ESP8266 based on CMake.
To compile / flash examples you will have to have the development 
environment setup as described at https://github.com/rzajac/esp-dev-env.

## Examples.

- [AES Encrypt/Decrypt](examples/aes)
- [Blink LED](examples/blink)
- [Conditional LED blink](examples/blink_cond)
- [Custom config](examples/config)
- [Evens](examples/event_bus)
- [JSON](examples/json)
- [Timer](examples/timers)

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

which will install the libraries, header files and scripts in appropriate 
places in `$ESPROOT`.

## License.

[Apache License Version 2.0](LICENSE) unless stated otherwise.
