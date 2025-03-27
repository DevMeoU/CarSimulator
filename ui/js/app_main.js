import { updateTime } from './time/time.js';
import { initSignalControl } from './controls/signalControl.js';
import { initGearControl } from './controls/gearControl.js';
import OdoDisplay from './display/odoDisplay.js';
import { updateBatteryIcon } from './display/batteryDisplay.js';
import { initCsvHandler } from './handlers/csvHandler.js';

const odoDisplay = new OdoDisplay();


document.addEventListener('DOMContentLoaded', () => {
    // Gọi hàm xử lý dữ liệu ở đây
    initCsvHandler();
    initGearControl();
    initSignalControl();
    updateTime();
    updateBatteryIcon(20);
    odoDisplay.update(0);
});