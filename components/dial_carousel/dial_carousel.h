#pragma once

#include <array>
#include <cstdint>

#include "esphome/core/component.h"
#include "lvgl.h"

namespace esphome {
namespace dial_carousel {

enum class CarouselDirection : uint8_t {
  UP = 0,
  DOWN = 1,
};

struct CarouselSlotRefs {
  lv_obj_t *container{nullptr};
  lv_obj_t *icon{nullptr};
  lv_obj_t *title{nullptr};
};

struct CarouselSlotLayout {
  int x{0};
  int y{0};
  int w{40};
  int opa_text{0};
  int opa_title{0};
  int opa_bg{0};
  int opa_icon{96};
};

class DialCarousel : public Component {
 public:
  void set_slot(size_t index, lv_obj_t *container, lv_obj_t *icon, lv_obj_t *title);
  void set_lateral_title_opa_max(int value) { this->lateral_title_opa_max_ = value; }
  void set_titles_are_overlays(bool value) { this->titles_are_overlays_ = value; }
  void set_center_bar(lv_obj_t *bar) { this->center_bar_ = bar; }
  void set_center_sub(lv_obj_t *sub) { this->center_sub_ = sub; }

  bool is_animating() const { return this->animating_; }

  CarouselSlotLayout compute_static_layout(size_t slot_index, int pill_shift_max) const;
  void apply_container_geometry(size_t slot_index, const CarouselSlotLayout &layout) const;
  void apply_title_layout(size_t slot_index, const CarouselSlotLayout &layout) const;

  void apply_animation_frame(int32_t progress, CarouselDirection direction, int pill_shift_max);

  bool start_animation(CarouselDirection direction, int pill_shift_max, void (*on_ready)(void *), void *context);

 protected:
  CarouselSlotLayout compute_layout_(size_t slot_index, float off, int pill_shift_max) const;
  int compute_lateral_title_opa_(int mix) const;

  std::array<CarouselSlotRefs, 5> slots_{};
  int lateral_title_opa_max_{0};
  bool titles_are_overlays_{false};
  lv_obj_t *center_bar_{nullptr};
  lv_obj_t *center_sub_{nullptr};

  bool animating_{false};
  CarouselDirection anim_direction_{CarouselDirection::UP};
  int anim_pill_shift_max_{54};
  lv_anim_t anim_{};
  void (*on_ready_)(void *){nullptr};
  void *on_ready_context_{nullptr};

  static void anim_exec_cb_(void *var, int32_t progress);
  static void anim_ready_cb_(lv_anim_t *anim);
};

}  // namespace dial_carousel
}  // namespace esphome
