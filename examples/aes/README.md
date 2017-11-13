## AES CBC encrypt / decrypt.

Demonstrates how to encrypt/decrypt buffer of data using AES CBC.
Use [gen_aes.sh](../../bin/gen_aes.sh) script to generate your own keys.

## Flashing.

```
$ cd build
$ cmake ..
$ make aes_ex_flash
$ miniterm.py /dev/ttyUSB0 74880
```
