## Conditional blink example

Blink LED connected to GPIO2 only if GPIO3 is HIGH.

Demonstrates how to:
- set pin state to LOW or HIGH, 
- read pin state,
- use RX pin as GPIO3,
- use timers.

## Flashing

```
$ cd build
$ cmake ..
$ make blink_cond_flash
$ miniterm.py /dev/ttyUSB0 74880
```
