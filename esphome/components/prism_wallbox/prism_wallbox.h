#pragma once

#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "esphome/components/select/select.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/mqtt/mqtt_client.h"

namespace esphome {
namespace prism_wallbox {

static const float MIN_VOLTAGE = 190.0;
static const float MIN_CURRENT = 6.0;
static const float MAX_CURRENT = 32.0;
static const float MIN_POWER = MIN_VOLTAGE * MIN_CURRENT;


class PrismWallbox : public Component {
  public:
    // Config
    std::string mqtt_prefix_;
    std::string port_;
    uint8_t qos_;
    uint8_t phases_ = 0;
    bool power_meter_;
    std::string max_current_command_topic_;
    std::string current_control_command_topic_;
    std::string mode_command_topic_;
    // Variable
    float grid_power_ = 0;
    float voltage_ = 0;
    float current_control_ = MAX_CURRENT;
    float power_ = 0;
    float current_ = 0;
    float power_current_ratio_ = 0;
    float solar_delta_power_ = 0;
    std::string prism_current_payload_ = "";
    std::string prism_state_ = "";
    std::string prism_mode_ = "";
    std::string mode_ = "";
    std::string mode_default_ = "";
    // Entities
    sensor::Sensor* power_grid_sensor_;
    sensor::Sensor* temperature_sensor_;
    sensor::Sensor* voltage_sensor_;
    text_sensor::TextSensor* state_sensor_;
    number::Number* max_current_number_;
    sensor::Sensor* current_control_sensor_;
    sensor::Sensor* session_energy_sensor_;
    sensor::Sensor* total_energy_sensor_;
    sensor::Sensor* power_sensor_;
    sensor::Sensor* current_sensor_;
    sensor::Sensor* session_time_sensor_;
    text_sensor::TextSensor* mode_text_sensor_;
    select::Select* mode_select_;
    sensor::Sensor* phases_sensor_;
    number::Number* solar_delta_power_number_;
    number::Number* power_control_number_;
    number::Number* power_control_modifier_number_;

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
    void set_current_control_sensor(sensor::Sensor *current_control_sensor) { current_control_sensor_ = current_control_sensor; }
    void set_session_energy_sensor(sensor::Sensor *session_energy_sensor) { session_energy_sensor_ = session_energy_sensor; }
    void set_total_energy_sensor(sensor::Sensor *total_energy_sensor) { total_energy_sensor_ = total_energy_sensor; }
    void set_power_sensor(sensor::Sensor *power_sensor) { power_sensor_ = power_sensor; }
    void set_current_sensor(sensor::Sensor *current_sensor) { current_sensor_ = current_sensor; }
    void set_session_time_sensor(sensor::Sensor *session_time_sensor) { session_time_sensor_ = session_time_sensor; }
    void set_mode_text_sensor(text_sensor::TextSensor *mode_text_sensor) { mode_text_sensor_ = mode_text_sensor; }
    void set_mode_default(std::string value);
    void set_mode_select(select::Select *mode_select) { mode_select_ = mode_select; }
    void set_phases_sensor(sensor::Sensor *phases_sensor) { phases_sensor_ = phases_sensor; }
    void set_solar_delta_power_default(float  default_value) { solar_delta_power_ = default_value; }
    void set_solar_delta_power_number(number::Number *solar_delta_power_number) { solar_delta_power_number_ = solar_delta_power_number; }
    void set_power_control_number(number::Number *power_control_number) { power_control_number_ = power_control_number; }
    void set_power_control_modifier_number(number::Number *power_control_modifier_number) { power_control_modifier_number_ = power_control_modifier_number; }
    void dump_config() override;
    void setup() override;
    void on_grid_power_change(float value);
    void on_voltage_change(float value);
    void on_prism_state_change(std::string value);
    void on_prism_mode_change(std::string value);
    void set_current_control(float value);
    void on_power_change(float value);
    void on_current_change(float value);
    void on_prism_control_current_change(std::string value);
    void set_mode(std::string value);
    void set_prism_mode(std::string value);
    void update_settings(std::string old_mode = "none", std::string old_prism_mode = "none", std::string old_prism_state = "none");
    void search_phases();
    void set_phases(uint8_t value);
    void set_power_control_modifier(float value);
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


class SolarDeltaPower : public number::Number, public Parented<PrismWallbox> {
  public:
    SolarDeltaPower();

  protected:
    void control(float value) override;
};


class PowerControl : public number::Number, public Parented<PrismWallbox> {
  public:
    PowerControl();

  protected:
    void control(float value) override;
};


class PowerControlModifier : public number::Number, public Parented<PrismWallbox> {
  public:
    PowerControlModifier();

  protected:
    void control(float value) override;
};


}  // namespace prism_wallbox
}  // namespace esphome
