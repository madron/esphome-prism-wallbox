#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace prism_wallbox {

class PrismWallbox : public Component {
  public:
    std::string mqtt_prefix_;
    int port_;
    sensor::Sensor* power_grid_sensor_;

    void set_mqtt_prefix(std::string mqtt_prefix) { mqtt_prefix_ = mqtt_prefix; }
    void set_port(int port) { port_ = port; }
    void set_power_grid_sensor(int port) { port_ = port; }
    void dump_config() override;
    void setup() override;
    void set_power_grid_sensor(sensor::Sensor *power_grid_sensor) { power_grid_sensor_ = power_grid_sensor; }
};

}  // namespace prism_wallbox
}  // namespace esphome
