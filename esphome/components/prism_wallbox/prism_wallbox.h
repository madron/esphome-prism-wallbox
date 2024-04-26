#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/mqtt/mqtt_client.h"

namespace esphome {
namespace prism_wallbox {

class PrismWallbox : public Component {
  public:
    std::string mqtt_prefix_;
    std::string  port_;
    uint8_t qos_;
    bool power_meter_;
    float grid_power_;
    float voltage_;
    sensor::Sensor* power_grid_sensor_;
    sensor::Sensor* temperature_sensor_;
    sensor::Sensor* voltage_sensor_;

    void set_mqtt_prefix(std::string mqtt_prefix) { mqtt_prefix_ = mqtt_prefix; }
    void set_port(int port) { port_ = std::to_string(port); }
    void set_qos(uint8_t qos) { this->qos_ = qos; }
    void set_power_meter(bool power_meter) { this->power_meter_ = power_meter; }
    void set_power_grid_sensor(sensor::Sensor *power_grid_sensor) { power_grid_sensor_ = power_grid_sensor; }
    void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
    void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage_sensor_ = voltage_sensor; }
    void dump_config() override;
    void setup() override;
    void on_grid_power_change(float value);
    void on_voltage_change(float value);
};

}  // namespace prism_wallbox
}  // namespace esphome
