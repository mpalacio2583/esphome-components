#include "esphome/core/log.h"
#include "miot_rs1bb.h"

namespace esphome {
namespace miot_rs1bb {

static const char *const TAG = "miot_rs1bb";

// Object ids as broadcast by the RS1BB. These are NOT the standard dentra
// MIID constants (MIID_FLOODING = 0x1014, MIID_BATTERY = 0x100A), which is why
// this device needs its own platform instead of reusing miot_sjws01lm.
static constexpr auto MIID_RS1BB_FLOODING = static_cast<miot::MIID>(0x4806);
static constexpr auto MIID_RS1BB_BATTERY = static_cast<miot::MIID>(0x4C03);

void MiotRS1BB::dump_config() {
  this->dump_config_(TAG, "RS1BB");
  LOG_BINARY_SENSOR("  ", "Flooding", this);
}

void MiotRS1BB::process_flooding_(const miot::BLEObject &obj) {
  // 1 byte: 0x00 = dry, non-zero (0x01) = water detected.
  const auto flooding = obj.get_bool();
  if (flooding.has_value()) {
    this->publish_state(*flooding);
  }
}

void MiotRS1BB::process_battery_(const miot::BLEObject &obj) {
  // 1 byte: battery level in percent (e.g. 0x64 = 100%).
  const auto level = obj.get_uint8();
  if (level.has_value() && this->battery_level_ != nullptr) {
    this->battery_level_->publish_state(*level);
  }
}

bool MiotRS1BB::process_object_(const miot::BLEObject &obj) {
  switch (obj.id) {
    case MIID_RS1BB_FLOODING:
      this->process_flooding_(obj);
      break;

    case MIID_RS1BB_BATTERY:
      this->process_battery_(obj);
      break;

    default:
      return this->process_default_(obj);
  }
  return true;
}

}  // namespace miot_rs1bb
}  // namespace esphome
