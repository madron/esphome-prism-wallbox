import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_PORT, CONF_QOS, CONF_TEMPERATURE
from esphome.const import UNIT_CELSIUS, UNIT_WATT
from esphome.const import DEVICE_CLASS_POWER, DEVICE_CLASS_TEMPERATURE
from esphome.const import STATE_CLASS_MEASUREMENT

AUTO_LOAD = ['sensor', 'mqtt']

CONF_MQTT_PREFIX = 'mqtt_prefix'
CONF_POWER_GRID = 'power_grid'
CONF_POWER_METER = 'power_meter'
ICON_POWER_GRID = 'mdi:transmission-tower'


prism_wallbox_ns = cg.esphome_ns.namespace('prism_wallbox')
PrismWallbox = prism_wallbox_ns.class_('PrismWallbox', cg.Component)


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(PrismWallbox),
        cv.Required(CONF_MQTT_PREFIX): cv.alphanumeric,
        cv.Optional(CONF_PORT, default=1): cv.int_range(min=1, max=2),
        cv.Optional(CONF_QOS, default=0): cv.mqtt_qos,
        cv.Optional(CONF_POWER_METER, default=True): cv.boolean,
        cv.Optional(CONF_POWER_GRID): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            icon=ICON_POWER_GRID,
        ),
        cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add(var.set_mqtt_prefix(config[CONF_MQTT_PREFIX]))
    cg.add(var.set_port(config[CONF_PORT]))
    cg.add(var.set_qos(config[CONF_QOS]))
    cg.add(var.set_power_meter(config[CONF_POWER_METER]))
    if CONF_POWER_GRID in config:
        conf = config[CONF_POWER_GRID]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_power_grid_sensor(sens))
    if CONF_TEMPERATURE in config:
        conf = config[CONF_TEMPERATURE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temperature_sensor(sens))
