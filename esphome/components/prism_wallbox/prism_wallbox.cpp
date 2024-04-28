// #include <sstream>
#include "prism_wallbox.h"
#include "esphome/core/log.h"

namespace esphome {
namespace prism_wallbox {

#define PTABLE_SIZE 100

static const char *const TAG = "prism_wallbox";

void PrismWallbox::dump_config() {
  ESP_LOGCONFIG(TAG, "PrismWallbox");
  ESP_LOGCONFIG(TAG, "  mqtt_prefix: %s", this->mqtt_prefix_.c_str());
  ESP_LOGCONFIG(TAG, "  port: %s", this->port_.c_str());
  ESP_LOGCONFIG(TAG, "  qos: %d", this->qos_);
}

void PrismWallbox::setup() {
  this->mode_ = "Normal";
  this->max_current_command_topic_ = this->mqtt_prefix_ + "/" + this->port_ + "/command/set_current_user";
  this->control_current_command_topic_ = this->mqtt_prefix_ + "/" + this->port_ + "/command/set_current_limit";
  this->mode_command_topic_ = this->mqtt_prefix_ + "/" + this->port_ + "/command/set_mode";
  this->set_mode("Normal");
  if (this->solar_delta_power_number_ != nullptr) {
    this->solar_delta_power_number_->publish_state(this->solar_delta_power_);
  }
  // power_meter_
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
      this->on_prism_state_change(payload);
    },
    this->qos_
  );
  // raw_mode
  mqtt::global_mqtt_client->subscribe(
    this->mqtt_prefix_ + "/" + this->port_ + "/mode",
    [this](const std::string &topic, const std::string &payload) {
      this->on_prism_mode_change(payload);
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
  // control_current_sensor_
  if (this->control_current_sensor_ != nullptr) {
    mqtt::global_mqtt_client->subscribe(
      this->control_current_command_topic_,
      [this](const std::string &topic, const std::string &payload) {
        auto val = parse_number<float>(payload);
        if (!val.has_value()) {
          ESP_LOGW(TAG, "Can't convert '%s' to number! (%s)", payload.c_str(), topic.c_str());
          this->control_current_sensor_->publish_state(NAN);
          return;
        }
        this->control_current_sensor_->publish_state(*val);
      },
      this->qos_
    );
  }
  // session_energy_sensor_
  if (this->session_energy_sensor_ != nullptr) {
    mqtt::global_mqtt_client->subscribe(
      this->mqtt_prefix_ + "/" + this->port_ + "/wh",
      [this](const std::string &topic, const std::string &payload) {
        auto val = parse_number<float>(payload);
        if (!val.has_value()) {
          ESP_LOGW(TAG, "Can't convert '%s' to number! (%s)", payload.c_str(), topic.c_str());
          this->session_energy_sensor_->publish_state(NAN);
          return;
        }
        this->session_energy_sensor_->publish_state(*val);
      },
      this->qos_
    );
  }
  // total_energy_sensor_
  if (this->total_energy_sensor_ != nullptr) {
    mqtt::global_mqtt_client->subscribe(
      this->mqtt_prefix_ + "/" + this->port_ + "/wh_total",
      [this](const std::string &topic, const std::string &payload) {
        auto val = parse_number<float>(payload);
        if (!val.has_value()) {
          ESP_LOGW(TAG, "Can't convert '%s' to number! (%s)", payload.c_str(), topic.c_str());
          this->total_energy_sensor_->publish_state(NAN);
          return;
        }
        this->total_energy_sensor_->publish_state(*val);
      },
      this->qos_
    );
  }
  // power
  mqtt::global_mqtt_client->subscribe(
    this->mqtt_prefix_ + "/" + this->port_ + "/w",
    [this](const std::string &topic, const std::string &payload) {
      auto val = parse_number<float>(payload);
      if (!val.has_value()) {
        ESP_LOGW(TAG, "Can't convert '%s' to number! (%s)", payload.c_str(), topic.c_str());
        this->on_power_change(NAN);
        return;
      }
      this->on_power_change(*val);
    },
    this->qos_
  );
  // current
  mqtt::global_mqtt_client->subscribe(
    this->mqtt_prefix_ + "/" + this->port_ + "/amp",
    [this](const std::string &topic, const std::string &payload) {
      auto val = parse_number<float>(payload);
      if (!val.has_value()) {
        ESP_LOGW(TAG, "Can't convert '%s' to number! (%s)", payload.c_str(), topic.c_str());
        this->on_current_change(NAN);
        return;
      }
      this->on_current_change(*val / 1000);
    },
    this->qos_
  );
  // session_time_sensor_
  if (this->session_time_sensor_ != nullptr) {
    mqtt::global_mqtt_client->subscribe(
      this->mqtt_prefix_ + "/" + this->port_ + "/session_time",
      [this](const std::string &topic, const std::string &payload) {
        auto val = parse_number<float>(payload);
        if (!val.has_value()) {
          ESP_LOGW(TAG, "Can't convert '%s' to number! (%s)", payload.c_str(), topic.c_str());
          this->session_time_sensor_->publish_state(NAN);
          return;
        }
        this->session_time_sensor_->publish_state(*val);
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

void PrismWallbox::on_prism_state_change(std::string value) {
  // 1 -> Unplugged
  // 2 -> Waiting
  // 3 -> Charging
  // 4 -> Pause
  std::string state = "";
  bool update_needed = false;
  if (value == "1") {
    state = "Unplugged";
  }
  else if (value == "2") {
    state = "Waiting";
  }
  else if (value == "3") {
    state = "Charging";
  }
  else if (value == "4") {
    state = "Pause";
  }
  else {
    state = value;
  }
  if (state != this->prism_state_) {
    update_needed = true;
  }
  ESP_LOGW(TAG, "on_prism_state_change %s -> %s - update_needed: %d", this->prism_state_.c_str(), state.c_str(), update_needed);
  this->prism_state_ = state;
  if (this->state_sensor_ != nullptr) {
    this->state_sensor_->publish_state(this->prism_state_);
  }
  if (update_needed) this->update_settings();
}

void PrismWallbox::on_prism_mode_change(std::string value) {
  // 1 -> Solar
  // 2 -> Normal
  // 3 -> Pause
  // 7 -> Low power
  std::string prism_mode;
  bool update_needed = false;
  if (value == "1") {
    prism_mode = "Solar";
  }
  else if (value == "2") {
    prism_mode = "Normal";
  }
  else if (value == "3") {
    prism_mode = "Pause";
  }
  else if (value == "7") {
    prism_mode = "Low power";
  }
  else {
    ESP_LOGW(TAG, "Unknown prism mode '%s'", value.c_str());
    return;
  }
  if (prism_mode != this->prism_mode_) {
    update_needed = true;
  }
  ESP_LOGW(TAG, "on_prism_mode_change %s -> %s - update_needed: %d", this->prism_mode_.c_str(), prism_mode.c_str(), update_needed);
  this->prism_mode_ = prism_mode;
  if (this->mode_text_sensor_ != nullptr) {
    this->mode_text_sensor_->publish_state(this->prism_mode_);
  }
  if (update_needed)  this->update_settings();
}

void PrismWallbox::set_control_current(float value) {
  char buffer[4];
  std::snprintf(buffer, 4, "%.1f", value);
  std::string payload(buffer);
  mqtt::global_mqtt_client->publish(this->control_current_command_topic_, payload, this->qos_, false);
  this->control_current_ = value;
}

void PrismWallbox::set_phases(uint8_t value) {
  this->phases_ = value;
  this->power_current_ratio_ = this->phases_ * this->voltage_;
  if (this->phases_sensor_ != nullptr) {
    if (this->phases_ == 0) {
      this->phases_sensor_->publish_state(NAN);
    }
    else {
      this->phases_sensor_->publish_state(this->phases_);
    }
  }
}

void PrismWallbox::on_voltage_change(float value) {
  this->voltage_ = value;
  this->search_phases();
  this->power_current_ratio_ = this->phases_ * this->voltage_;
  if (this->voltage_sensor_ != nullptr) {
        this->voltage_sensor_->publish_state(value);
  }
}

void PrismWallbox::on_power_change(float value) {
  this->power_ = value;
  this->search_phases();
  if (this->power_sensor_ != nullptr) {
        this->power_sensor_->publish_state(value);
  }
}

void PrismWallbox::on_current_change(float value) {
  this->current_ = value;
  this->search_phases();
  if (this->current_sensor_ != nullptr) {
        this->current_sensor_->publish_state(value);
  }
}

void PrismWallbox::search_phases() {
  if (this->phases_ == 0) {
    ESP_LOGW(TAG, "search_phases - voltage: %f - power: %f - current: %f", this->voltage_, this->power_, this->current_);
    if (this->voltage_ > MIN_VOLTAGE && this->power_ >= MIN_POWER && this->current_ >= MIN_CURRENT) {
      this->phases_ = (int) this->power_ / this->current_ / this->voltage_ + 0.5;
      this->set_phases(this->phases_);
    }
  }
}

void PrismWallbox::set_mode(std::string value) {
  bool update_needed = false;
  if (value != this->mode_) {
    update_needed = true;
  }
  ESP_LOGW(TAG, "set_mode %s -> %s - update_needed: %d", this->mode_.c_str(), value.c_str(), update_needed);
  this->mode_ = value;
  if (this->mode_select_ != nullptr) {
    this->mode_select_->publish_state(value);
  }
  if (update_needed)  this->update_settings();
}

void PrismWallbox::set_prism_mode(std::string value) {
  std::string payload;
  if (value == "Solar") {
    payload = "1";
  }
  else if (value == "Normal") {
    payload = "2";
  }
  else if (value == "Pause") {
    payload = "3";
  }
  else {
    payload = value;
  }
  mqtt::global_mqtt_client->publish(this->mode_command_topic_, payload, this->qos_, false);
}

void PrismWallbox::update_settings() {
  ESP_LOGW(TAG, "update_settings - mode: '%s' - prism_mode: '%s'  - prism_state: '%s'", this->mode_.c_str(), this->prism_mode_.c_str(), this->prism_state_.c_str());
  // prism_state
  if (this->prism_state_ == "Unplugged") {
    this->set_phases(0);
  }
  // mode
  if (this->mode_ == "Solar") {
    this->set_prism_mode("Normal");
    if (this->phases_ == 0) {
      this->set_control_current(MIN_CURRENT);
    }
  }
  else if (this->mode_ == "Normal") {
    this->set_prism_mode("Normal");
    this->set_control_current(MAX_CURRENT);
  }
  else if (this->mode_ == "Pause") {
    this->set_prism_mode("Pause");
  }
}


MaxCurrent::MaxCurrent() {}
void MaxCurrent::control(float value) {
  mqtt::global_mqtt_client->publish(this->parent_->max_current_command_topic_, std::to_string(value), this->parent_->qos_, false);
}


Mode::Mode() {}
void Mode::control(const std::string &value) {
  this->parent_->set_mode(value);
}


SolarDeltaPower::SolarDeltaPower() {}
void SolarDeltaPower::control(float value) {
  this->parent_->solar_delta_power_ = value;
}


}  // namespace prism_wallbox
}  // namespace esphome
