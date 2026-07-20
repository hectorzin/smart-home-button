import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ENTITY_ID, CONF_ID, CONF_NAME

CODEOWNERS = []
DEPENDENCIES = []

dial_lights_ns = cg.esphome_ns.namespace("dial_lights")
DialLights = dial_lights_ns.class_("DialLights", cg.Component)

LIGHT_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_ENTITY_ID): cv.string,
        cv.Required(CONF_NAME): cv.string,
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
        cg.add(var.add_light(light[CONF_ENTITY_ID], light[CONF_NAME]))
