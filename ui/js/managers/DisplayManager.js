import AirConditionDisplay from '../display/AirConditionDisplay.js';
import BatteryDisplay from '../display/BatteryDisplay.js';
import BatteryTempDisplay from '../display/BatteryTempDisplay.js';
import DoorLockDisplay from '../display/DoorLockDisplay.js';
import EnginePowerDisplay from '../display/EnginePowerDisplay.js';
import FanDisplay from '../display/FanDisplay.js';
import GearDisplay from '../display/GearDisplay.js';
import ModeDisplay from '../display/ModeDisplay.js';
import OdoDisplay from '../display/OdoDisplay.js';
import PedalDisplay from '../display/PedalDisplay.js';
import SafetySystemDisplay from '../display/SafetySystemDisplay.js';
import SignalDisplay from '../display/SignalDisplay.js';
import SpeedDisplay from '../display/SpeedDisplay.js';
import WarningDisplay from '../display/WarningDisplay.js';
import NumberUtils from '../utils/NumberUtils.js';

class DisplayManager {
    constructor() {
        this.batteryDisplay = new BatteryDisplay();
        this.batteryTempDisplay = new BatteryTempDisplay();
        this.signalDisplay = new SignalDisplay();
        this.speedDisplay = new SpeedDisplay();
        this.gearDisplay = new GearDisplay();
        this.pedalDisplay = new PedalDisplay();
        this.odoDisplay = new OdoDisplay();
        this.doorLockDisplay = new DoorLockDisplay();
        this.airConditionDisplay = new AirConditionDisplay();
        this.fanDisplay = new FanDisplay();
        this.modeDisplay = new ModeDisplay();
        this.warningDisplay = new WarningDisplay();
        this.enginePowerDisplay = new EnginePowerDisplay();
        this.safetySystemDisplay = new SafetySystemDisplay();
    }

    updates(data) {
        this.batteryDisplay.update(NumberUtils.roundBattery(data.battery));
        this.batteryTempDisplay.update(NumberUtils.roundTemperature(data.battery_temp));
        this.signalDisplay.update(data.signal_left, data.signal_right);
        this.speedDisplay.update(NumberUtils.roundSpeed(data.speed));
        this.gearDisplay.update(data.gear);
        this.pedalDisplay.update(NumberUtils.roundPedal(data.brake), NumberUtils.roundPedal(data.gas));
        this.odoDisplay.update(NumberUtils.roundDistance(data.distance_traveled), NumberUtils.roundDistance(data.estimated_distance));
        this.doorLockDisplay.update(data.door_lock);
        this.airConditionDisplay.update(data.air_condition);
        this.fanDisplay.update(data.fan);
        this.modeDisplay.update(data.mode);
        this.warningDisplay.update(data.warning);
        this.enginePowerDisplay.update(NumberUtils.roundEngine(data.engine_power), NumberUtils.roundEngine(data.engine_torque), NumberUtils.roundTemperature(data.engine_temp));
        this.safetySystemDisplay.update(data.abs_active, data.esp_active)
    }
}

export default DisplayManager;