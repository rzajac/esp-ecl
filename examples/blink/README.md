## ESP8266 blink example.

Blink LED connected to GPIO2.

Demonstrates how to:
- set pin state to LOW or HIGH, 
- read pin state,
- use of SDK timers. 

## Flashing.

```
$ cd build
$ cmake ..
$ make blink_ex_flash
$ miniterm.py /dev/ttyUSB0 74880
```
