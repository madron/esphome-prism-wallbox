#pragma once

#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "esphome/components/select/select.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/mqtt/mqtt_client.h"

namespace esphome {
namespace prism_wallbox {

static const float const MIN_VOLTAGE = 200.0;
static const float const MIN_CURRENT = 6.0;
static const float const MAX_CURRENT = 32.0;
static const float const MIN_POWER = 1200.0;


class PrismWallbox : public Component {
  public:
    // Config
    std::string mqtt_prefix_;
    std::string port_;
    uint8_t qos_;
    uint8_t phases_ = 0;
    bool power_meter_;
    std::string max_current_command_topic_;
    std::string control_current_command_topic_;
    std::string mode_command_topic_;
    // Variable
    float grid_power_ = 0;
    float voltage_ = 0;
    float control_current_ = MAX_CURRENT;
    float power_ = 0;
    float current_ = 0;
    std::string prism_state_ = "";
    std::string prism_mode_ = "";
    std::string mode_ = "";
    // Entities
    sensor::Sensor* power_grid_sensor_;
    sensor::Sensor* temperature_sensor_;
    sensor::Sensor* voltage_sensor_;
    text_sensor::TextSensor* state_sensor_;
    number::Number* max_current_number_;
    sensor::Sensor* control_current_sensor_;
    sensor::Sensor* session_energy_sensor_;
    sensor::Sensor* total_energy_sensor_;
    sensor::Sensor* power_sensor_;
    sensor::Sensor* current_sensor_;
    sensor::Sensor* session_time_sensor_;
    text_sensor::TextSensor* mode_text_sensor_;
    select::Select* mode_select_;
    sensor::Sensor* phases_sensor_;

    float get_setup_priority() const override { return setup_priority::AFTER_CONNECTION; }
    void set_mqtt_prefix(std::string mqtt_prefix) { mqtt_prefix_ = mqtt_prefix; }
    void set_port(int port) { port_ = std::to_string(port); }
    void set_qos(uint8_t qos) { this->qos_ = qos; }
    void set_power_meter(bool power_meter) { this->power_meter_ = power_meter; }
    void set_power_grid_sensor(sensor::Sensor *power_grid_sensor) { power_grid_sensor_ = power_grid_sensor; }
    void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
    void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage_sensor_ = voltage_sensor; }
    void set_state_sensor(text_sensor::TextSensor *state_sensor) { state_sensor_ = state_sensor; }
    void set_max_current_number(number::Number *max_current_number) { max_current_number_ = max_current_number; }
    void set_control_current_sensor(sensor::Sensor *control_current_sensor) { control_current_sensor_ = control_current_sensor; }
    void set_session_energy_sensor(sensor::Sensor *session_energy_sensor) { session_energy_sensor_ = session_energy_sensor; }
    void set_total_energy_sensor(sensor::Sensor *total_energy_sensor) { total_energy_sensor_ = total_energy_sensor; }
    void set_power_sensor(sensor::Sensor *power_sensor) { power_sensor_ = power_sensor; }
    void set_current_sensor(sensor::Sensor *current_sensor) { current_sensor_ = current_sensor; }
    void set_session_time_sensor(sensor::Sensor *session_time_sensor) { session_time_sensor_ = session_time_sensor; }
    void set_mode_text_sensor(text_sensor::TextSensor *mode_text_sensor) { mode_text_sensor_ = mode_text_sensor; }
    void set_mode_select(select::Select *mode_select) { mode_select_ = mode_select; }
    void set_phases_sensor(sensor::Sensor *phases_sensor) { phases_sensor_ = phases_sensor; }
    void dump_config() override;
    void setup() override;
    void on_grid_power_change(float value);
    void on_voltage_change(float value);
    void on_prism_state_change(std::string value);
    void on_prism_mode_change(std::string value);
    void set_control_current(float value);
    void on_power_change(float value);
    void on_current_change(float value);
    void set_mode(std::string value);
    void set_prism_mode(std::string value);
    void update_settings();
    void search_phases();
    void set_phases(uint8_t value);
};


class MaxCurrent : public number::Number, public Parented<PrismWallbox> {
  public:
    MaxCurrent();

  protected:
    void control(float value) override;
};


class Mode : public select::Select, public Parented<PrismWallbox> {
  public:
    Mode();

  protected:
    void control(const std::string &value) override;
};


}  // namespace prism_wallbox
}  // namespace esphome
