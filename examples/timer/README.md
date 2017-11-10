## Timers example.

Example program which is calling a callback but each time the call period is doubled.

Demonstrates how to:
- Use timer helpers. 
- Pass data to callbacks called by timers.

## Flashing

```
$ cd build
$ cmake ..
$ make timer_flash
$ miniterm.py /dev/ttyUSB0 74880
```
