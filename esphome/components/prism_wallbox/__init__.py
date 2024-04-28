import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number, sensor, select, text_sensor
from esphome.const import CONF_CURRENT, CONF_ID, CONF_MAX_CURRENT, CONF_MODE, CONF_PORT, CONF_POWER, CONF_QOS, CONF_STATE, CONF_TEMPERATURE, CONF_VOLTAGE
from esphome.const import UNIT_AMPERE, UNIT_CELSIUS, UNIT_EMPTY, UNIT_SECOND, UNIT_VOLT, UNIT_WATT, UNIT_WATT_HOURS
from esphome.const import DEVICE_CLASS_CURRENT, DEVICE_CLASS_DURATION, DEVICE_CLASS_ENERGY, DEVICE_CLASS_ENERGY_STORAGE, DEVICE_CLASS_POWER, DEVICE_CLASS_TEMPERATURE, DEVICE_CLASS_VOLTAGE
from esphome.const import STATE_CLASS_MEASUREMENT, STATE_CLASS_TOTAL_INCREASING

AUTO_LOAD = ['sensor', 'text_sensor', 'number', 'select', 'mqtt']

CONF_MQTT_PREFIX = 'mqtt_prefix'
CONF_POWER_GRID = 'power_grid'
CONF_POWER_METER = 'power_meter'
CONF_CURRENT_CONTROL = 'current_control'
CONF_POWER_CONTROL = 'power_control'
CONF_POWER_CONTROL_MODIFIER = 'power_control_modifier'
CONF_SESSION_ENERGY = 'session_energy'
CONF_TOTAL_ENERGY = 'total_energy'
CONF_SESSION_TIME = 'session_time'
CONF_PHASES = 'phases'
CONF_MODE_DEFAULT = 'mode_default'
CONF_MODE_TEXT = 'mode_text'
CONF_SOLAR_DELTA_POWER = 'solar_delta_power'
CONF_SOLAR_DELTA_POWER_DEFAULT = 'solar_delta_power_default'
ICON_POWER_GRID = 'mdi:transmission-tower'
ICON_EV_PLUG = 'mdi:ev-plug-type2'
ICON_CLOCK = 'mdi:clock-time-eight-outline'

MAX_CURRENT_MIN = 6
MAX_CURRENT_MAX = 32
MAX_CURRENT_STEP = 1
SOLAR_DELTA_POWER_MIN = 0
SOLAR_DELTA_POWER_MAX = 500
SOLAR_DELTA_POWER_STEP = 50
POWER_CONTROL_MIN = 0
POWER_CONTROL_MAX = 22000
POWER_CONTROL_STEP = 1

MODE_OPTIONS = [
    'Normal',
    'Solar',
    'Pause',
]


prism_wallbox_ns = cg.esphome_ns.namespace('prism_wallbox')
PrismWallbox = prism_wallbox_ns.class_('PrismWallbox', cg.Component)
MaxCurrent = prism_wallbox_ns.class_('MaxCurrent', number.Number)
Mode = prism_wallbox_ns.class_('Mode', select.Select)
SolarDeltaPower = prism_wallbox_ns.class_('SolarDeltaPower', number.Number)
PowerControl = prism_wallbox_ns.class_('PowerControl', number.Number)
PowerControlModifier = prism_wallbox_ns.class_('PowerControlModifier', number.Number)


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
        cv.Optional(CONF_CURRENT_CONTROL): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_POWER_CONTROL): number.number_schema(
            PowerControl,
            unit_of_measurement=UNIT_WATT,
            device_class=DEVICE_CLASS_POWER,
        ),
        cv.Optional(CONF_POWER_CONTROL_MODIFIER): number.number_schema(
            PowerControlModifier,
            unit_of_measurement=UNIT_WATT,
            device_class=DEVICE_CLASS_POWER,
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
        cv.Optional(CONF_MODE_TEXT): text_sensor.text_sensor_schema(
        ),
        cv.Optional(CONF_MODE_DEFAULT, default='Normal'): cv.one_of('normal', 'solar', 'pause', lower=True),
        cv.Optional(CONF_MODE): select.select_schema(
            Mode,
        ),
        cv.Optional(CONF_PHASES): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_SOLAR_DELTA_POWER_DEFAULT, default=0): cv.float_range(min=SOLAR_DELTA_POWER_MIN, max=SOLAR_DELTA_POWER_MAX),
        cv.Optional(CONF_SOLAR_DELTA_POWER): number.number_schema(
            SolarDeltaPower,
            unit_of_measurement=UNIT_WATT,
            device_class=DEVICE_CLASS_POWER,
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
    if CONF_CURRENT_CONTROL in config:
        conf = config[CONF_CURRENT_CONTROL]
        entity = await sensor.new_sensor(conf)
        cg.add(var.set_current_control_sensor(entity))
    if CONF_POWER_CONTROL in config:
        conf = config[CONF_POWER_CONTROL]
        entity = await number.new_number(conf, min_value=POWER_CONTROL_MIN, max_value=POWER_CONTROL_MAX, step=POWER_CONTROL_STEP)
        await cg.register_parented(entity, config[CONF_ID])
        cg.add(var.set_power_control_number(entity))
    if CONF_POWER_CONTROL_MODIFIER in config:
        conf = config[CONF_POWER_CONTROL_MODIFIER]
        entity = await number.new_number(conf, min_value=-POWER_CONTROL_MAX, max_value=POWER_CONTROL_MAX, step=POWER_CONTROL_STEP)
        await cg.register_parented(entity, config[CONF_ID])
        cg.add(var.set_power_control_modifier_number(entity))
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
    if CONF_MODE_TEXT in config:
        conf = config[CONF_MODE_TEXT]
        entity = await text_sensor.new_text_sensor(conf)
        cg.add(var.set_mode_text_sensor(entity))
    if CONF_MODE_DEFAULT in config:
        conf = config[CONF_MODE_DEFAULT]
        cg.add(var.set_mode_default(conf))
    if CONF_MODE in config:
        conf = config[CONF_MODE]
        entity = await select.new_select(conf, options=MODE_OPTIONS)
        await cg.register_parented(entity, config[CONF_ID])
        cg.add(var.set_mode_select(entity))
    if CONF_PHASES in config:
        conf = config[CONF_PHASES]
        entity = await sensor.new_sensor(conf)
        cg.add(var.set_phases_sensor(entity))
    if CONF_SOLAR_DELTA_POWER_DEFAULT in config:
        conf = config[CONF_SOLAR_DELTA_POWER_DEFAULT]
        cg.add(var.set_solar_delta_power_default(conf))
    if CONF_SOLAR_DELTA_POWER in config:
        conf = config[CONF_SOLAR_DELTA_POWER]
        entity = await number.new_number(conf, min_value=SOLAR_DELTA_POWER_MIN, max_value=SOLAR_DELTA_POWER_MAX, step=SOLAR_DELTA_POWER_STEP)
        await cg.register_parented(entity, config[CONF_ID])
        cg.add(var.set_solar_delta_power_number(entity))
