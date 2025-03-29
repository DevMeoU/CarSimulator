import AirConditionDisplay from '../display/AirConditionDisplay.js';
import BatteryDisplay from '../display/batteryDisplay.js';
import BatteryTempDisplay from '../display/batteryTempDisplay.js';
import DoorLockDisplay from '../display/DoorLockDisplay.js';
import FanDisplay from '../display/FanDisplay.js';
import GearDisplay from '../display/gearDisplay.js';
import ModeDisplay from '../display/modeDisplay.js';
import OdoDisplay from '../display/odoDisplay.js';
import PedalDisplay from '../display/pedalDisplay.js';
import SignalDisplay from '../display/signalDisplay.js';
import SpeedDisplay from '../display/speedDisplay.js';

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
    }

    updates(data) {
        this.batteryDisplay.update(data.battery);
        this.batteryTempDisplay.update(data.battery_temp);
        this.signalDisplay.update(data.signal_left, data.signal_right);
        this.speedDisplay.update(data.speed);
        this.gearDisplay.update(data.gear);
        this.pedalDisplay.update(data.brake, data.gas);
        this.odoDisplay.update(data.distance_traveled, data.estimated_distance);
        this.doorLockDisplay.update(data.door_lock);
        this.airConditionDisplay.update(data.air_condition);
        this.fanDisplay.update(data.wind);
        this.modeDisplay.update(data.mode);
    }
}

export default DisplayManager;