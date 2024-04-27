import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number, sensor, text_sensor
from esphome.const import CONF_CURRENT, CONF_ID, CONF_MAX_CURRENT, CONF_PORT, CONF_POWER, CONF_QOS, CONF_STATE, CONF_TEMPERATURE, CONF_VOLTAGE
from esphome.const import UNIT_AMPERE, UNIT_CELSIUS, UNIT_SECOND, UNIT_VOLT, UNIT_WATT, UNIT_WATT_HOURS
from esphome.const import DEVICE_CLASS_CURRENT, DEVICE_CLASS_DURATION, DEVICE_CLASS_ENERGY, DEVICE_CLASS_ENERGY_STORAGE, DEVICE_CLASS_POWER, DEVICE_CLASS_TEMPERATURE, DEVICE_CLASS_VOLTAGE
from esphome.const import STATE_CLASS_MEASUREMENT, STATE_CLASS_TOTAL_INCREASING

AUTO_LOAD = ['sensor', 'text_sensor', 'number', 'mqtt']

CONF_MQTT_PREFIX = 'mqtt_prefix'
CONF_POWER_GRID = 'power_grid'
CONF_POWER_METER = 'power_meter'
CONF_CONTROL_CURRENT = 'control_current'
CONF_SESSION_ENERGY = 'session_energy'
CONF_TOTAL_ENERGY = 'total_energy'
CONF_SESSION_TIME = 'session_time'
ICON_POWER_GRID = 'mdi:transmission-tower'
ICON_EV_PLUG = 'mdi:ev-plug-type2'
ICON_CLOCK = 'mdi:clock-time-eight-outline'

MAX_CURRENT_MIN = 6
MAX_CURRENT_MAX = 32
MAX_CURRENT_STEP = 1


prism_wallbox_ns = cg.esphome_ns.namespace('prism_wallbox')
PrismWallbox = prism_wallbox_ns.class_('PrismWallbox', cg.Component)
MaxCurrent = prism_wallbox_ns.class_('MaxCurrent', number.Number)


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
        cv.Optional(CONF_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_STATE): text_sensor.text_sensor_schema(
            icon=ICON_EV_PLUG,
        ),
        cv.Optional(CONF_MAX_CURRENT): number.number_schema(
            MaxCurrent,
            unit_of_measurement=UNIT_AMPERE,
            device_class=DEVICE_CLASS_CURRENT,
        ),
        cv.Optional(CONF_CONTROL_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_SESSION_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_ENERGY_STORAGE,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
        cv.Optional(CONF_TOTAL_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
        cv.Optional(CONF_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_SESSION_TIME): sensor.sensor_schema(
            unit_of_measurement=UNIT_SECOND,
            icon=ICON_CLOCK,
            accuracy_decimals=0,
            state_class=STATE_CLASS_TOTAL_INCREASING,
            device_class=DEVICE_CLASS_DURATION,
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
        entity = await sensor.new_sensor(conf)
        cg.add(var.set_power_grid_sensor(entity))
    if CONF_TEMPERATURE in config:
        conf = config[CONF_TEMPERATURE]
        entity = await sensor.new_sensor(conf)
        cg.add(var.set_temperature_sensor(entity))
    if CONF_VOLTAGE in config:
        conf = config[CONF_VOLTAGE]
        entity = await sensor.new_sensor(conf)
        cg.add(var.set_voltage_sensor(entity))
    if CONF_STATE in config:
        conf = config[CONF_STATE]
        entity = await text_sensor.new_text_sensor(conf)
        cg.add(var.set_state_sensor(entity))
    if CONF_MAX_CURRENT in config:
        conf = config[CONF_MAX_CURRENT]
        entity = await number.new_number(conf, min_value=MAX_CURRENT_MIN, max_value=MAX_CURRENT_MAX, step=MAX_CURRENT_STEP)
        await cg.register_parented(entity, config[CONF_ID])
        cg.add(var.set_max_current_number(entity))
    if CONF_CONTROL_CURRENT in config:
        conf = config[CONF_CONTROL_CURRENT]
        entity = await sensor.new_sensor(conf)
        cg.add(var.set_control_current_sensor(entity))
    if CONF_SESSION_ENERGY in config:
        conf = config[CONF_SESSION_ENERGY]
        entity = await sensor.new_sensor(conf)
        cg.add(var.set_session_energy_sensor(entity))
    if CONF_TOTAL_ENERGY in config:
        conf = config[CONF_TOTAL_ENERGY]
        entity = await sensor.new_sensor(conf)
        cg.add(var.set_total_energy_sensor(entity))
    if CONF_POWER in config:
        conf = config[CONF_POWER]
        entity = await sensor.new_sensor(conf)
        cg.add(var.set_power_sensor(entity))
    if CONF_CURRENT in config:
        conf = config[CONF_CURRENT]
        entity = await sensor.new_sensor(conf)
        cg.add(var.set_current_sensor(entity))
    if CONF_SESSION_TIME in config:
        conf = config[CONF_SESSION_TIME]
        entity = await sensor.new_sensor(conf)
        cg.add(var.set_session_time_sensor(entity))
