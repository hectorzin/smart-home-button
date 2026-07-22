import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.lvgl.types import lv_obj_t
from esphome.const import CONF_ID

CODEOWNERS = []
DEPENDENCIES = ["lvgl"]
AUTO_LOAD = ["lvgl"]

MULTI_CONF = True

CONF_SLOTS = "slots"
CONF_CONTAINER = "container"
CONF_ICON = "icon"
CONF_TITLE = "title"
CONF_CENTER_BAR = "center_bar"
CONF_CENTER_SUB = "center_sub"
CONF_LATERAL_TITLE_OPA_MAX = "lateral_title_opa_max"

SLOT_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_CONTAINER): cv.use_id(lv_obj_t),
        cv.Required(CONF_ICON): cv.use_id(lv_obj_t),
        cv.Required(CONF_TITLE): cv.use_id(lv_obj_t),
    }
)

dial_carousel_ns = cg.esphome_ns.namespace("dial_carousel")
DialCarousel = dial_carousel_ns.class_("DialCarousel", cg.Component)
CarouselDirection = dial_carousel_ns.enum("CarouselDirection", True)
CarouselDirectionUP = CarouselDirection.UP
CarouselDirectionDOWN = CarouselDirection.DOWN

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DialCarousel),
        cv.Required(CONF_SLOTS): cv.All(cv.ensure_list(SLOT_SCHEMA), cv.Length(min=5, max=5)),
        cv.Required(CONF_CENTER_BAR): cv.use_id(lv_obj_t),
        cv.Required(CONF_CENTER_SUB): cv.use_id(lv_obj_t),
        cv.Optional(CONF_LATERAL_TITLE_OPA_MAX, default=0): cv.int_range(
            min=0, max=255
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    for index, slot in enumerate(config[CONF_SLOTS]):
        container = await cg.get_variable(slot[CONF_CONTAINER])
        icon = await cg.get_variable(slot[CONF_ICON])
        title = await cg.get_variable(slot[CONF_TITLE])
        cg.add(var.set_slot(index, container, icon, title))

    center_bar = await cg.get_variable(config[CONF_CENTER_BAR])
    center_sub = await cg.get_variable(config[CONF_CENTER_SUB])
    cg.add(var.set_center_bar(center_bar))
    cg.add(var.set_center_sub(center_sub))
    cg.add(var.set_lateral_title_opa_max(config[CONF_LATERAL_TITLE_OPA_MAX]))
