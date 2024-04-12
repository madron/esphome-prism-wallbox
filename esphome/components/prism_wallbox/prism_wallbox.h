#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace prism_wallbox {

class PrismWallbox : public Component {
  public:
    binary_sensor::BinarySensor* input_;
    std::string mqtt_prefix_;
    int port_;

    void set_mqtt_prefix(std::string mqtt_prefix) { mqtt_prefix_ = mqtt_prefix; }
    void set_port(int port) { port_ = port; }
    void dump_config() override;
    void setup() override;
};

}  // namespace prism_wallbox
}  // namespace esphome
