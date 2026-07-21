#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace dial_lights {

class DialLights : public Component {
 public:
  void add_light(const std::string &entity_id, const std::string &name, text_sensor::TextSensor *state = nullptr);
  void setup() override;

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

 protected:
  struct LightEntry {
    std::string entity_id;
    std::string name;
    text_sensor::TextSensor *state{nullptr};
    bool state_valid{false};
    bool is_on{false};
  };

  const LightEntry &active_entry_() const;
  void on_state_(size_t index, const std::string &value);

  std::vector<LightEntry> lights_;
  size_t active_index_{0};
};

}  // namespace dial_lights
}  // namespace esphome
