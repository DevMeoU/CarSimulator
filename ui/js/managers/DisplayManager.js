import BatteryDisplay from '../display/batteryDisplay.js';
import BatteryTempDisplay from '../display/batteryTempDisplay.js';
import SignalDisplay from '../display/signalDisplay.js';
import SpeedDisplay from '../display/speedDisplay.js';
import GearDisplay from '../display/gearDisplay.js';
import PedalDisplay from '../display/pedalDisplay.js';
import OdoDisplay from '../display/odoDisplay.js';

class DisplayManager {
    constructor() {
        this.batteryDisplay = new BatteryDisplay();
        this.batteryTempDisplay = new BatteryTempDisplay();
        this.signalDisplay = new SignalDisplay();
        this.speedDisplay = new SpeedDisplay();
        this.gearDisplay = new GearDisplay();
        this.pedalDisplay = new PedalDisplay();
        this.odoDisplay = new OdoDisplay();
    }

    updates(data) {
        this.batteryDisplay.update(data.battery);
        this.batteryTempDisplay.update(data.battery_temp);
        this.signalDisplay.update(data.signal_left, data.signal_right);
        this.speedDisplay.update(data.speed);
        this.gearDisplay.update(data.gear);
        this.pedalDisplay.update(data.brake, data.gas);
        this.odoDisplay.update(data.distance_traveled, data.estimated_distance);
    }
}

export default DisplayManager;