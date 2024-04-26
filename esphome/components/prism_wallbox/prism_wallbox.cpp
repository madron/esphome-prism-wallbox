#include "prism_wallbox.h"
#include "esphome/core/log.h"

namespace esphome {
namespace prism_wallbox {

static const char *const TAG = "prism_wallbox";

void PrismWallbox::dump_config() {
  ESP_LOGCONFIG(TAG, "PrismWallbox");
  ESP_LOGCONFIG(TAG, "  mqtt_prefix: %s", this->mqtt_prefix_.c_str());
  ESP_LOGCONFIG(TAG, "  port: %d", this->port_);
  ESP_LOGCONFIG(TAG, "  qos: %d", this->qos_);

}

void PrismWallbox::setup() {
  // power_meter_
  if (this->power_meter_) {
    this->power_grid_topic_ = this->mqtt_prefix_ + "/energy_data/power_grid";
    mqtt::global_mqtt_client->subscribe(
      this->power_grid_topic_,
      [this](const std::string &topic, const std::string &payload) {
        auto val = parse_number<float>(payload);
        if (!val.has_value()) {
          ESP_LOGW(TAG, "Can't convert '%s' to number! (%s)", payload.c_str(), topic.c_str());
          this->on_grid_power_change(NAN);
          return;
        }
        val = *val * (float)-1;
        this->on_grid_power_change(*val);
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


}  // namespace prism_wallbox
}  // namespace esphome
