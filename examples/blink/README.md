## Blink example.

Blink LED connected to GPIO2.

Demonstrates how to:
- set pin state to LOW or HIGH, 
- read pin state,
- use of SDK timers. 

## Flashing.

```
$ cd build
$ cmake ..
$ make ex_blink
$ miniterm.py /dev/ttyUSB0 74880
```
