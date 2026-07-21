import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.components.sensor as sensor
import esphome.components.text_sensor as text_sensor
from esphome.const import CONF_ENTITY_ID, CONF_ID, CONF_NAME

CODEOWNERS = []
DEPENDENCIES = ["api", "sensor", "text_sensor"]

CONF_STATE_SENSOR = "state_sensor"
CONF_MODES_SENSOR = "modes_sensor"
CONF_BRIGHTNESS_SENSOR = "brightness_sensor"
CONF_COLOR_SENSOR = "color_sensor"

dial_lights_ns = cg.esphome_ns.namespace("dial_lights")
DialLights = dial_lights_ns.class_("DialLights", cg.Component)

LIGHT_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_ENTITY_ID): cv.string,
        cv.Required(CONF_NAME): cv.string,
        cv.Optional(CONF_STATE_SENSOR): cv.use_id(text_sensor.TextSensor),
        cv.Optional(CONF_MODES_SENSOR): cv.use_id(text_sensor.TextSensor),
        cv.Optional(CONF_BRIGHTNESS_SENSOR): cv.use_id(sensor.Sensor),
        cv.Optional(CONF_COLOR_SENSOR): cv.use_id(text_sensor.TextSensor),
    }
)


def _config_schema(value):
    lights = cv.All(cv.ensure_list(LIGHT_SCHEMA), cv.Length(min=1))(value)
    return {
        CONF_ID: cv.declare_id(DialLights)("dial_lights_id"),
        "lights": lights,
    }


CONFIG_SCHEMA = _config_schema


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    for light in config["lights"]:
        entity = light[CONF_ENTITY_ID]
        name = light[CONF_NAME]
        state = None
        modes = None
        brightness = None
        color = None
        if CONF_STATE_SENSOR in light:
            state = await cg.get_variable(light[CONF_STATE_SENSOR])
        if CONF_MODES_SENSOR in light:
            modes = await cg.get_variable(light[CONF_MODES_SENSOR])
        if CONF_BRIGHTNESS_SENSOR in light:
            brightness = await cg.get_variable(light[CONF_BRIGHTNESS_SENSOR])
        if CONF_COLOR_SENSOR in light:
            color = await cg.get_variable(light[CONF_COLOR_SENSOR])
        cg.add(var.add_light(entity, name, state, modes, brightness, color))
