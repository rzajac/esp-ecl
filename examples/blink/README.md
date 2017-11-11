## ESP8266 blink example

Blink LED connected to GPIO2.

Demonstrates how to:
- set pin state to LOW or HIGH, 
- read pin state,
- use timers. 

## Flashing

```
$ cd build
$ cmake ..
$ make blink_flash
$ miniterm.py /dev/ttyUSB0 74880
```
