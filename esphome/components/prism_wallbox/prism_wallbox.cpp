#include "prism_wallbox.h"
#include "esphome/core/log.h"

namespace esphome {
namespace prism_wallbox {

static const char *const TAG = "prism_wallbox";

void PrismWallbox::dump_config() {
  ESP_LOGCONFIG(TAG, "PrismWallbox");
  ESP_LOGCONFIG(TAG, "  mqtt_prefix: %s", this->mqtt_prefix_.c_str());
  ESP_LOGCONFIG(TAG, "  port: %s", this->port_.c_str());
  ESP_LOGCONFIG(TAG, "  qos: %d", this->qos_);
}

void PrismWallbox::setup() {
  this->max_current_command_topic_ = this->mqtt_prefix_ + "/" + this->port_ + "/command/set_current_user";
  this->control_current_command_topic_ = this->mqtt_prefix_ + "/" + this->port_ + "/command/set_current_limit";
  // power_meter_
  this->on_grid_power_change(NAN);
  if (this->power_meter_) {
    mqtt::global_mqtt_client->subscribe(
      this->mqtt_prefix_ + "/energy_data/power_grid",
      [this](const std::string &topic, const std::string &payload) {
        auto val = parse_number<float>(payload);
        if (!val.has_value()) {
          ESP_LOGW(TAG, "Can't convert '%s' to number! (%s)", payload.c_str(), topic.c_str());
          this->on_grid_power_change(NAN);
          return;
        }
        this->on_grid_power_change(*val);
      },
      this->qos_
    );
  }
  // temperature_sensor_
  if (this->temperature_sensor_ != nullptr) {
    mqtt::global_mqtt_client->subscribe(
      this->mqtt_prefix_ + "/0/info/temperature/core",
      [this](const std::string &topic, const std::string &payload) {
        auto val = parse_number<float>(payload);
        if (!val.has_value()) {
          ESP_LOGW(TAG, "Can't convert '%s' to number! (%s)", payload.c_str(), topic.c_str());
          this->temperature_sensor_->publish_state(NAN);
          return;
        }
        this->temperature_sensor_->publish_state(*val);
      },
      this->qos_
    );
  }
  // voltage
  this->on_voltage_change(NAN);
  mqtt::global_mqtt_client->subscribe(
    this->mqtt_prefix_ + "/" + this->port_ + "/volt",
    [this](const std::string &topic, const std::string &payload) {
      auto val = parse_number<float>(payload);
      if (!val.has_value()) {
        ESP_LOGW(TAG, "Can't convert '%s' to number! (%s)", payload.c_str(), topic.c_str());
        this->on_voltage_change(NAN);
        return;
      }
      this->on_voltage_change(*val);
    },
    this->qos_
  );
  // raw_state
  mqtt::global_mqtt_client->subscribe(
    this->mqtt_prefix_ + "/" + this->port_ + "/state",
    [this](const std::string &topic, const std::string &payload) {
      this->on_raw_state_change(payload);
    },
    this->qos_
  );
  // max_current_number_
  if (this->max_current_number_ != nullptr) {
    mqtt::global_mqtt_client->subscribe(
      this->mqtt_prefix_ + "/" + this->port_ + "/user_amp",
      [this](const std::string &topic, const std::string &payload) {
        auto val = parse_number<float>(payload);
        if (!val.has_value()) {
          ESP_LOGW(TAG, "Can't convert '%s' to number! (%s)", payload.c_str(), topic.c_str());
          this->max_current_number_->publish_state(NAN);
          return;
        }
        this->max_current_number_->publish_state(*val);
      },
      this->qos_
    );
  }
  // control_current_number_
  if (this->control_current_number_ != nullptr) {
    mqtt::global_mqtt_client->subscribe(
      this->control_current_command_topic_,
      [this](const std::string &topic, const std::string &payload) {
        auto val = parse_number<float>(payload);
        if (!val.has_value()) {
          ESP_LOGW(TAG, "Can't convert '%s' to number! (%s)", payload.c_str(), topic.c_str());
          this->control_current_number_->publish_state(NAN);
          return;
        }
        this->control_current_number_->publish_state(*val);
      },
      this->qos_
    );
  }
}

void PrismWallbox::on_grid_power_change(float value) {
  this->grid_power_ = value;
  if (this->power_grid_sensor_ != nullptr) {
        this->power_grid_sensor_->publish_state(value);
  }
}

void PrismWallbox::on_voltage_change(float value) {
  this->voltage_ = value;
  if (this->voltage_sensor_ != nullptr) {
        this->voltage_sensor_->publish_state(value);
  }
}

void PrismWallbox::on_raw_state_change(std::string value) {
  this->raw_state_ = value;
  // 1 -> Unplugged
  // 2 -> Waiting
  // 3 -> Charging
  // 4 -> Pause
  std::string text_state;
  if (value == "1") {
    text_state = "Unplugged";
  }
  else if (value == "2") {
    text_state = "Waiting";
  }
  else if (value == "3") {
    text_state = "Charging";
  }
  else if (value == "4") {
    text_state = "Pause";
  }
  else {
    text_state = value;
  }
  this->text_state_ = text_state;
  if (this->state_sensor_ != nullptr) {
    this->state_sensor_->publish_state(text_state);
  }
}


MaxCurrent::MaxCurrent() {}

void MaxCurrent::control(float value) {
  mqtt::global_mqtt_client->publish(this->parent_->max_current_command_topic_, std::to_string(value), this->parent_->qos_, false);
}


ControlCurrent::ControlCurrent() {}

void ControlCurrent::control(float value) {
  mqtt::global_mqtt_client->publish(this->parent_->control_current_command_topic_, std::to_string(value), this->parent_->qos_, false);
}

}  // namespace prism_wallbox
}  // namespace esphome
