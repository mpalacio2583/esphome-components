#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../miot/miot.h"

namespace esphome {
namespace miot_rs1bb {

class MiotRS1BB : public miot::MiotComponent, public binary_sensor::BinarySensor {
 public:
  // Product ID as broadcast by the Linptech RS1BB (seen in the logs as 3F0F).
  // The miot base validates this against the incoming MiBeacon and drops
  // anything that does not match, so it must be set.
  MiotRS1BB() { this->product_id_ = 0x3F0F; }

  void dump_config() override;

 protected:
  bool process_object_(const miot::BLEObject &obj) override;

  void process_flooding_(const miot::BLEObject &obj);
  void process_battery_(const miot::BLEObject &obj);
};

}  // namespace miot_rs1bb
}  // namespace esphome
