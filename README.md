## ESP8266 Common Libraries (ECL)

The repository is a collection of small but useful libraries I use in 
many projects gathered in one place: 

- [esp_gpio](src) - fast pin manipulation library.
- [esp_sdo](src) - use RX pin as the GPIO3.
- [esp_json](src) - makes JSON manipulation easier.
- [esp_cfg](src) - read / write custom configuration structures from / to flash.
- [esp_aes](src) - AES CBC encryption / decryption.
- [esp_tim](src) - Collection of helpers to deal with timers.
- [esp_eb](src) - Simple event bus.

## Build environment.

This library is part of my build system for ESP8266 based on CMake.
To run examples you will have to have the ESP development environment setup 
as described at https://github.com/rzajac/esp-dev-env.

## Examples.

- [Blink LED](examples/blink)
- [Conditional LED blink](examples/blink_cond)
- [JSON](examples/json)
- [Custom config](examples/cfg)
- [Encrypt/Decrypt](examples/aes)
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

which will install the library and headers in `$ESPROOT`.

## License.

[Apache License Version 2.0](LICENSE) unless stated otherwise.
