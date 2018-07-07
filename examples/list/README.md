## Timers example.

Example program which is calling a callback using timer but each time the 
call period is doubled.

Demonstrates how to:
- use timer helpers, 
- pass data to callbacks called by timers.

## Flashing.

```
$ cd build
$ cmake ..
$ make timer_ex_flash
$ miniterm.py /dev/ttyUSB0 74880
```
