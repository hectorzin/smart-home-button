#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "esphome/core/component.h"

namespace esphome {
namespace dial_lights {

class DialLights : public Component {
 public:
  void add_light(const std::string &entity_id, const std::string &name);

  size_t light_count() const { return this->lights_.size(); }
  size_t active_index() const { return this->active_index_; }
  void select_light(size_t index);
  void select_next();
  void select_previous();
  const std::string &active_name() const;
  const std::string &active_entity_id() const;
  const std::string &name_at(size_t index) const;

 protected:
  struct LightMetadata {
    std::string entity_id;
    std::string name;
  };

  std::vector<LightMetadata> lights_;
  size_t active_index_{0};
};

}  // namespace dial_lights
}  // namespace esphome
