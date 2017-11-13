## Conditional blink example.

Blink LED connected to GPIO2 only if GPIO3 is HIGH.

Demonstrates how to:
- set pin state to LOW or HIGH, 
- read pin state,
- use RX pin as GPIO3,
- use of SDK timers.

## Flashing.

```
$ cd build
$ cmake ..
$ make blink_cond_ex_flash
$ miniterm.py /dev/ttyUSB0 74880
```
