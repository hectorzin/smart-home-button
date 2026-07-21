#include "dial_lights.h"

namespace esphome {
namespace dial_lights {

namespace {
const std::string EMPTY_STRING;
}

void DialLights::add_light(
    const std::string &entity_id,
    const std::string &name,
    text_sensor::TextSensor *state) {
  this->lights_.push_back({entity_id, name, state, false, false});
}

void DialLights::setup() {
  for (size_t i = 0; i < this->lights_.size(); i++) {
    auto &light = this->lights_[i];
    if (light.state != nullptr) {
      light.state->add_on_state_callback([this, i](const std::string &value) { this->on_state_(i, value); });
    }
  }
}

const DialLights::LightEntry &DialLights::active_entry_() const { return this->lights_[this->active_index_]; }

void DialLights::on_state_(size_t index, const std::string &value) {
  auto &light = this->lights_[index];
  if (value == "on") {
    light.state_valid = true;
    light.is_on = true;
    return;
  }
  if (value == "off") {
    light.state_valid = true;
    light.is_on = false;
    return;
  }
  light.state_valid = false;
}

bool DialLights::active_has_valid_state() const {
  if (this->lights_.empty() || this->active_index_ >= this->lights_.size())
    return false;
  return this->active_entry_().state_valid;
}

bool DialLights::active_is_on() const {
  if (this->lights_.empty() || this->active_index_ >= this->lights_.size())
    return false;
  return this->active_entry_().is_on;
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
