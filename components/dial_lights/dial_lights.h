#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace dial_lights {

class DialLights : public Component {
 public:
  void add_light(const std::string &entity_id, const std::string &name, text_sensor::TextSensor *state = nullptr,
                 text_sensor::TextSensor *modes = nullptr, sensor::Sensor *brightness = nullptr,
                 text_sensor::TextSensor *color = nullptr);
  void setup() override;
  void load_active_snapshot();

  size_t light_count() const { return this->lights_.size(); }
  size_t active_index() const { return this->active_index_; }
  void select_light(size_t index);
  void select_next();
  void select_previous();
  const std::string &active_name() const;
  const std::string &active_entity_id() const;
  const std::string &name_at(size_t index) const;

  bool active_has_valid_state() const;
  bool active_is_on() const;
  bool active_supports_brightness() const;
  bool active_supports_rgb() const;
  bool active_brightness_valid() const;
  int active_brightness_percent() const;
  bool active_color_valid() const;
  int active_color_r() const;
  int active_color_g() const;
  int active_color_b() const;
  int active_color_h() const;

 protected:
  struct LightEntry {
    std::string entity_id;
    std::string name;
    text_sensor::TextSensor *state{nullptr};
    bool state_valid{false};
    bool is_on{false};
    text_sensor::TextSensor *modes{nullptr};
    bool supports_brightness{false};
    bool supports_rgb{false};
    sensor::Sensor *brightness{nullptr};
    bool brightness_valid{false};
    int brightness_percent{75};
    text_sensor::TextSensor *color{nullptr};
    bool color_valid{false};
    int color_r{169};
    int color_g{143};
    int color_b{255};
  };

  const LightEntry &active_entry_() const;
  void on_state_(size_t index, const std::string &value);
  void on_modes_(size_t index, const std::string &value);
  void on_brightness_(size_t index, float value);
  void on_color_(size_t index, const std::string &value);

  std::vector<LightEntry> lights_;
  size_t active_index_{0};
};

}  // namespace dial_lights
}  // namespace esphome
