## esp_cfg

Library for storing custom data structures on flash to keep it between 
resets / reboots. Depending on your configuration, linker script and 
data size you may have to customize `ESP_CFG_START_SECTOR` in `esp_config.h` 
file. By default it's set to sector `0xC` (one sector is 4096 bytes) which 
is located before user app (`0x10000`).  

By default user can use two configuration structures. You may change it by 
defining `ESP_CFG_NUMBER` or editing `esp_config.h` file (making this 
change requires changing `ESP_CFG_START_SECTOR`). 

See its documentation in [esp_cfg.h](include/esp_cfg.h) header file for more 
details.
