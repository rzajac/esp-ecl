## Save configuration between reboots.

Demonstrates how to:
- Read custom configuration structure from the flash.
- Write custom configuration structure to flash. 

## Flashing

```
$ cd build
$ cmake ..
$ make cfg_flash
$ miniterm.py /dev/ttyUSB0 74880
```
