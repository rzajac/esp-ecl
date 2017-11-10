## ESP8266 Blink Example

Blink LED connected to GPIO2.

Demonstrates how to:
- Set pin state to LOW or HIGH. 
- Read pin state.
- Use timers. 

## Flashing

```
$ cd build
$ cmake ..
$ make blink_flash
$ miniterm.py /dev/ttyUSB0 74880
```
