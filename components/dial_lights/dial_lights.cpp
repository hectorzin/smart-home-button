#include "dial_lights.h"

namespace esphome {
namespace dial_lights {

namespace {
const std::string EMPTY_STRING;
}

void DialLights::add_light(const std::string &entity_id, const std::string &name) {
  this->lights_.push_back({entity_id, name});
}

void DialLights::select_light(size_t index) {
  if (this->lights_.empty())
    return;
  this->active_index_ = index % this->lights_.size();
}

void DialLights::select_next() {
  if (!this->lights_.empty())
    this->active_index_ = (this->active_index_ + 1) % this->lights_.size();
}

void DialLights::select_previous() {
  if (!this->lights_.empty())
    this->active_index_ = (this->active_index_ + this->lights_.size() - 1) % this->lights_.size();
}

const std::string &DialLights::active_name() const { return this->name_at(this->active_index_); }

const std::string &DialLights::active_entity_id() const {
  if (this->lights_.empty())
    return EMPTY_STRING;
  return this->lights_[this->active_index_].entity_id;
}

const std::string &DialLights::name_at(size_t index) const {
  if (this->lights_.empty())
    return EMPTY_STRING;
  return this->lights_[index % this->lights_.size()].name;
}

}  // namespace dial_lights
}  // namespace esphome
