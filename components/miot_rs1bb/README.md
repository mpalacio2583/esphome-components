# Linptech RS1BB — Flood & Rain Sensor (MIOT platform)

ESPHome component for the **Linptech RS1BB** water leak / rain sensor
(sold as "Linptech Flood & Rain Sensor RS1", Xiaomi ecosystem, product id `0x3F0F`).

This device is **not** supported by ESPHome's native `xiaomi_ble`, nor by any of
dentra's existing device components: it is a different product from the Xiaomi
SJWS01LM, with a different product id and different MiBeacon object ids, so
`miot_sjws01lm` will not decode it.

Built on top of the [dentra MIOT platform](../miot), which handles the encrypted
MiBeacon decryption. This component only maps the device's object ids to entities.

## What it exposes

| Entity          | Type            | Source object id | Notes                              |
|-----------------|-----------------|------------------|------------------------------------|
| Flooding        | `binary_sensor` | `0x4806`         | `0x00` = dry, non-zero = water     |
| Battery level   | `sensor` (opt.) | `0x4C03`         | 1 byte, percent (e.g. `0x64`=100%) |

`device_class` is `moisture` (ON = water detected).

> These object ids were captured from a real RS1BB and are **not** dentra's
> standard MIID constants (`MIID_FLOODING = 0x1014`, `MIID_BATTERY = 0x100A`),
> which is why this device needs its own platform.

## Requirements

- An ESP32 running ESPHome with `esp32_ble_tracker`.
- The device's **bindkey**, obtained by pairing the sensor in the Mi Home app and
  extracting the key (e.g. via the Xiaomi Cloud token extractor, or dentra's
  `xiaomi_account` automatic extraction).

## Usage

```yaml
external_components:
  - source: github:///esphome-components
    components: [miot, xiaomi_account, miot_rs1bb]

esp32_ble_tracker:

# MIOT hub (required, can be empty)
miot:

binary_sensor:
  - platform: miot_rs1bb
    mac_address: "A4:C1:38:XX:XX:XX"
    bindkey: "<32-hex-char bindkey>"
    name: "Laundry Flood"
    # optional:
    battery_level:
      name: "Laundry Flood Battery"
```

## Notes / known behavior

- The RS1BB is a low-power device: it broadcasts **only on state change**
  (dry ↔ wet) and an occasional battery heartbeat (reported intervals up to
  ~50 min). After an ESP32 reboot the sensor stays in an unknown state until the
  first advertisement arrives.
- Battery voltage is not exposed because the device only broadcasts the battery
  percentage, not a separate voltage value.
- Verified against ESPHome 2026.5.x on `esp32dev` (esp-idf framework), publishing
  over MQTT to openHAB. Decoding matches the Mi Home app readings.

## Credits

Component by [@mpalacio2583](https://github.com/mpalacio2583).

Built on dentra's MIOT platform. Object ids reverse-engineered from `miot`'s
`on_miot_advertise` debug output on a real RS1BB.
