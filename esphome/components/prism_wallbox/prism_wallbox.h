#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/mqtt/mqtt_client.h"

namespace esphome {
namespace prism_wallbox {

class PrismWallbox : public Component {
  public:
    std::string mqtt_prefix_;
    int port_;
    uint8_t qos_;
    sensor::Sensor* power_grid_sensor_;
    std::string power_grid_topic_;

    void set_mqtt_prefix(std::string mqtt_prefix) { mqtt_prefix_ = mqtt_prefix; }
    void set_port(int port) { port_ = port; }
    void set_qos(uint8_t qos) { this->qos_ = qos; }
    void set_power_grid_sensor(sensor::Sensor *power_grid_sensor) { power_grid_sensor_ = power_grid_sensor; }
    void dump_config() override;
    void setup() override;
};

}  // namespace prism_wallbox
}  // namespace esphome
