## esp_cfg

Library gives simple interface to store/retrieve data on flash to for example
keep your application state between resets.  

## How it works.

By default user application is stored starting at flash address `0x10000` which
is 10 sectors from the beginning of the flash (one sector is `0x1000` bytes).

At flash address `0x0` starts bootloader but it does not take much space so 
we can use it to store our custom data. 

The library by default gives you 4 slots each 4096 bytes:

- slot 1 starting at sector `0xC` (flash address `0xC000`) 
- slot 2 starting at sector `0xD` (flash address `0xD000`) 
- slot 3 starting at sector `0xE` (flash address `0xE000`) 
- slot 4 starting at sector `0xF` (flash address `0xF000`) 

You can customize where the library stores your data with 
`ESP_CFG_START_SECTOR` and `ESP_CFG_NUMBER` in `esp_config.h` file.

See [example program](../../examples/cfg) and library documentation in 
[esp_cfg.h](include/esp_cfg.h) header file for more details.
