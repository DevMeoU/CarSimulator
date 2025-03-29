import AppManager from './managers/AppManager.js';
import GearControl from './controls/gearControl.js';
import ModeControl from './controls/modeControl.js';
import SignalControl from './controls/signalControl.js';
import OdoDisplay from './display/odoDisplay.js';
import PedalDisplay from './display/pedalDisplay.js';
import SpeedDisplay from './display/speedDisplay.js'
import DoorLockDisplay from './display/DoorLockDisplay.js';
import AirConditionDisplay from './display/AirConditionDisplay.js';
import FanDisplay from './display/FanDisplay.js';
import BatteryDisplay from './display/batteryDisplay.js';
import BatteryTempDisplay from './display/batteryTempDisplay.js';


class AppMain {

    constructor() {
        this.appManager = new AppManager();
        this.gearControl = new GearControl();
        this.modeControl = new ModeControl();
        this.signalControl = new SignalControl();
        this.airConditionDisplay = new AirConditionDisplay();
        this.fanDisplay = new FanDisplay();
        this.speedDisplay = new SpeedDisplay();
        this.odoDisplay = new OdoDisplay();
        this.pedalDisplay = new PedalDisplay();
        this.doorLockDisplay = new DoorLockDisplay();
        this.batteryDisplay = new BatteryDisplay();
        this.batteryTempDisplay = new BatteryTempDisplay();
    }
}

// Khởi tạo ứng dụng
const app = new AppMain();
