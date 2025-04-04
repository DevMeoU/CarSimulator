import AppManager from './managers/AppManager.js';
import GearControl from './controls/GearControl.js';
import ModeControl from './controls/ModeControl.js';
import SignalControl from './controls/signalControl.js';
import OdoDisplay from './display/OdoDisplay.js';
import PedalDisplay from './display/PedalDisplay.js';
import SpeedDisplay from './display/SpeedDisplay.js'
import DoorLockDisplay from './display/DoorLockDisplay.js';
import AirConditionDisplay from './display/AirConditionDisplay.js';
import FanDisplay from './display/FanDisplay.js';
import BatteryDisplay from './display/BatteryDisplay.js';
import BatteryTempDisplay from './display/BatteryTempDisplay.js';
import WarningDisplay from './display/WarningDisplay.js';


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
        this.warningDisplay = new WarningDisplay();
    }
}

// Khởi tạo ứng dụng
const app = new AppMain();
