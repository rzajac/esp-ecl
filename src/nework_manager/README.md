## Network manager.

Library makes easier to create TCP/IP clients. 

## Network states.

1.  **Start**            - start of the network related process.
2.  **Op mode change**   - change between station, AP or AP + station.
3.  **Auth mode change** - AP authentication mode changed.
4.  **Use DHCP**         - use DHCP to get IP.
5.  **Use static IP**    - use static IP.
6.  **Connecting**       - connection process started.
7.  **Connected**        - connected to access point.
8.  **Got IP**           - got IP address.
9.  **DHCP timeout**     - timeout getting IP.
10. **Disconnected**     - disconnected from access point.
11. **Stop**             - end of the network related process.

## WiFi.

### Connecting to WiFi which does not exist.

The NM will try to connect to WiFi `nm_wifi->recon_max` times. 
If maximum connection retries is reached NM will call 
`nm_err_cb` function, given to `nm_wifi_connect` 
with `NULL`, `ESP_E_WIF` and `REASON_NO_AP_FOUND`.

Before calling `nm_err_cb` NM will:

- remove all WiFi callbacks it registered during `nm_wifi_connect` execution,
- change `opmode` to `NULL_MODE`,

The `nm_wifi_connect` will set `nm_wifi->recon_max` to 1 if 0 is passed.

1 -> 6 -> 10

### Successful WiFi connection with DHCP client started or with static IP.

After successful connection the NM will call `nm_err_cb` function given to 
`nm_wifi_connect` with `ESP_OK` and 0. **Successful WiFi connection is the 
only case when this function is called with success error code.**

### Successful WiFi connection with DHCP client started but no DHCP server.

The NM will try to connect and request IP address `nm_wifi->recon_max` 
times. If max retries is reached NM will call `nm_err_cb` with 
`ESP_E_NET` error code.

Before calling `nm_err_cb` NM will:

- remove all WiFi callbacks it registered during `nm_wifi_connect` execution,
- change `opmode` to `NULL_MODE`,

### WiFi suddenly goes away.

The NM will call `nm_err_cb` with `ESP_E_WIF` and `EVENT_STAMODE_DISCONNECTED`
then it will try to reconnect `nm_wifi->recon_max`. On failure `nm_err_cb`
will be called again with `ESP_E_WIF` and disconnection reason (see 
`user_interface.h` file `REASON_*` enums).

If reconnection succeeds NM will call `nm_err_cb` function given to 
`nm_wifi_connect` with `ESP_OK` and 0.

## TCP/IP behaviour.

### Connecting to TCP/IP server but no route to server can be found.

The NM will call `nm_err_cb` with `ESP_E_NET` and `ESPCONN_CONN`. 

### Connecting to TCP/IP server which does not exist.

The NM will call `nm_err_cb` with `ESP_E_NET` and `ESPCONN_RST`. 



## All network cases.

1. AP connection cases:
    1. Connecting to not existing AP.
    1. Connecting to AP with wrong password.
    
1. After successful connection to AP.
    1. Trying to get IP with DHCP but no DHCP server running.
    1. Setting IP manually while someone already has the same IP.
    1. The AP suddenly does away.
    1. The AP disassociates the client.

1. 