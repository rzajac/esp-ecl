## Conditional Blink Example

Blink LED connected to GPIO2 only if GPIO3 is HIGH.

Demonstrates how to:
- Set pin state to LOW or HIGH. 
- Read pin state.
- Use RX pin as GPIO3.
- Use timers.

## Flashing

```
$ cd build
$ cmake ..
$ make blink_cond_flash
$ miniterm.py /dev/ttyUSB0 74880
```
