import AppManager from './managers/AppManager.js';
import GearControl from './controls/gearControl.js';
import ModeControl from './controls/modeControl.js';
import SignalControl from './controls/signalControl.js';

class AppMain {
    constructor() {
        this.appManager = new AppManager();
        this.gearControl = new GearControl();
        this.modeControl = new ModeControl();
        this.signalControl = new SignalControl();
    }
}

// Khởi tạo ứng dụng
const app = new AppMain();
