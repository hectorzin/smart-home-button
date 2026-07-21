#include "dial_lights.h"

#include <cctype>

namespace esphome {
namespace dial_lights {

namespace {
const std::string EMPTY_STRING;

void parse_color_modes(const std::string &raw, bool &supports_brightness, bool &supports_rgb) {
  supports_brightness = false;
  supports_rgb = false;

  std::string token;
  for (size_t i = 0; i <= raw.size(); i++) {
    const char c = (i < raw.size()) ? raw[i] : '\0';
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
      token += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    } else if (!token.empty()) {
      if (token == "brightness" || token == "color_temp" || token == "rgb" || token == "rgbw" || token == "rgbww") {
        supports_brightness = true;
      }
      if (token == "rgb" || token == "rgbw" || token == "rgbww") {
        supports_rgb = true;
      }
      token.clear();
    }
  }
}
}  // namespace

void DialLights::add_light(
    const std::string &entity_id,
    const std::string &name,
    text_sensor::TextSensor *state,
    text_sensor::TextSensor *modes) {
  this->lights_.push_back({entity_id, name, state, false, false, modes, false, false});
}

void DialLights::setup() {
  for (size_t i = 0; i < this->lights_.size(); i++) {
    auto &light = this->lights_[i];
    if (light.state != nullptr) {
      light.state->add_on_state_callback([this, i](const std::string &value) { this->on_state_(i, value); });
    }
    if (light.modes != nullptr) {
      light.modes->add_on_state_callback([this, i](const std::string &value) { this->on_modes_(i, value); });
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

void DialLights::on_modes_(size_t index, const std::string &value) {
  auto &light = this->lights_[index];
  light.supports_brightness = false;
  light.supports_rgb = false;
  parse_color_modes(value, light.supports_brightness, light.supports_rgb);
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

bool DialLights::active_supports_brightness() const {
  if (this->lights_.empty() || this->active_index_ >= this->lights_.size())
    return false;
  return this->active_entry_().supports_brightness;
}

bool DialLights::active_supports_rgb() const {
  if (this->lights_.empty() || this->active_index_ >= this->lights_.size())
    return false;
  return this->active_entry_().supports_rgb;
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
