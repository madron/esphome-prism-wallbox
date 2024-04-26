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
  // power_grid_sensor_
  if (this->power_grid_sensor_ != nullptr) {
    this->power_grid_topic_ = this->mqtt_prefix_ + "/energy_data/power_grid";
    mqtt::global_mqtt_client->subscribe(
      this->power_grid_topic_,
      [this](const std::string &topic, const std::string &payload) {
        auto val = parse_number<float>(payload);
        if (!val.has_value()) {
          ESP_LOGW(TAG, "Can't convert '%s' to number!", payload.c_str());
          this->power_grid_sensor_->publish_state(NAN);
          return;
        }
        val = *val * (float)-1;
        this->power_grid_sensor_->publish_state(*val);
      },
      this->qos_
    );
  }
}


}  // namespace prism_wallbox
}  // namespace esphome
