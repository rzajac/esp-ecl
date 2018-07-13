

## WiFi behaviour.

### Connecting to WiFi which does not exist.

The NM nm will try to connect to WiFi `nm_wifi->recon_max` times. If max 
retries will be reached NM will call `nm_err_cb` function, given to 
`nm_wifi_start` with `NULL`, `ESP_E_WIF` and `REASON_NO_AP_FOUND`.

Before calling `nm_err_cb` NM will:

- remove all WiFi callbacks it registered during `nm_wifi_start` execution,
- change `opmode` to `NULL_MODE`,

The `nm_wifi_start` will set `nm_wifi->recon_max` to 1 if 0 is passed.

## Successful WiFi connection with DHCP client started or with static IP.

After successful connection the NM will call `nm_err_cb` function given to 
`nm_wifi_start` with `ESP_OK` and 0. **Successful WiFi connection is the 
only case when this function is called with success error code.**

##### Successful WiFi connection with DHCP client started but no DHCP server.

The NM will try to connect and request IP address `nm_wifi->recon_max` 
times. If max retries is reached NM will call `nm_err_cb` with 
`ESP_E_NET` error code.

Before calling `nm_err_cb` NM will:

- remove all WiFi callbacks it registered during `nm_wifi_start` execution,
- change `opmode` to `NULL_MODE`,

##### WiFi suddenly goes away.

The NM will call `nm_err_cb` with `ESP_E_WIF` and `EVENT_STAMODE_DISCONNECTED`
then it will try to reconnect `nm_wifi->recon_max`. On failure `nm_err_cb`
will be called again with `ESP_E_WIF` and disconnection reason (see 
`user_interface.h` file `REASON_*` enums).

If reconnection succeeds NM will call `nm_err_cb` function given to 
`nm_wifi_start` with `ESP_OK` and 0.

