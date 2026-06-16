import esphome.codegen as cg
from esphome.components import binary_sensor
from esphome.const import DEVICE_CLASS_MOISTURE
from .. import miot  # pylint: disable=relative-beyond-top-level

# Linptech RS1BB Flood & Rain sensor.
# Built on top of dentra's MIOT platform (the `miot` base component does the
# MiBeacon decryption; this platform only maps the device's object ids).

CODEOWNERS = ["@mauriciopalacio"]
AUTO_LOAD = ["miot"]

miot_rs1bb_ns = cg.esphome_ns.namespace("miot_rs1bb")
MiotRS1BB = miot_rs1bb_ns.class_(
    "MiotRS1BB", miot.MiotComponent, binary_sensor.BinarySensor
)

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(
    MiotRS1BB, device_class=DEVICE_CLASS_MOISTURE
).extend(miot.MIOT_BLE_DEVICE_SCHEMA)


async def to_code(config):
    """Code generation entry point"""
    await miot.new_binary_sensor_device(config)
