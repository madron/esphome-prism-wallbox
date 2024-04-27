#pragma once

#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/mqtt/mqtt_client.h"

namespace esphome {
namespace prism_wallbox {


class PrismWallbox : public Component {
  public:
    std::string mqtt_prefix_;
    std::string port_;
    uint8_t qos_;
    bool power_meter_;
    float grid_power_;
    float voltage_;
    std::string max_current_command_topic_;
    std::string limit_current_command_topic_;
    std::string raw_state_;
    std::string text_state_;
    sensor::Sensor* power_grid_sensor_;
    sensor::Sensor* temperature_sensor_;
    sensor::Sensor* voltage_sensor_;
    text_sensor::TextSensor* state_sensor_;
    number::Number* max_current_sensor_;
    number::Number* limit_current_sensor_;

    float get_setup_priority() const override { return setup_priority::AFTER_CONNECTION; }
    void set_mqtt_prefix(std::string mqtt_prefix) { mqtt_prefix_ = mqtt_prefix; }
    void set_port(int port) { port_ = std::to_string(port); }
    void set_qos(uint8_t qos) { this->qos_ = qos; }
    void set_power_meter(bool power_meter) { this->power_meter_ = power_meter; }
    void set_power_grid_sensor(sensor::Sensor *power_grid_sensor) { power_grid_sensor_ = power_grid_sensor; }
    void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
    void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage_sensor_ = voltage_sensor; }
    void set_state_sensor(text_sensor::TextSensor *state_sensor) { state_sensor_ = state_sensor; }
    void set_max_current_sensor(number::Number *max_current_sensor) { max_current_sensor_ = max_current_sensor; }
    void set_limit_current_sensor(number::Number *limit_current_sensor) { limit_current_sensor_ = limit_current_sensor; }
    void dump_config() override;
    void setup() override;
    void on_grid_power_change(float value);
    void on_voltage_change(float value);
    void on_raw_state_change(std::string value);
};


class MaxCurrent : public number::Number, public Parented<PrismWallbox> {
  public:
    MaxCurrent();

  protected:
    void control(float value) override;
};


class LimitCurrent : public number::Number, public Parented<PrismWallbox> {
  public:
    LimitCurrent();

  protected:
    void control(float value) override;
};


}  // namespace prism_wallbox
}  // namespace esphome
