import { updateTime } from './time/time.js';
import { initSignalControl } from './controls/signalControl.js';
import { initGearControl } from './controls/gearControl.js';
import OdoDisplay from './display/odoDisplay.js';
import { updateBatteryIcon } from './display/batteryDisplay.js';
import { parseCSV } from './handlers/csvHandler.js';

const odoDisplay = new OdoDisplay();


document.addEventListener('DOMContentLoaded', () => {
    fetch('../data/storage.csv')
    .then(response => {
        if (!response.ok) {
        throw new Error('Network response was not ok');
        }
        return response.text();
    })
    .then(text => {
        const parsedData = parseCSV(text);
        console.log('Dữ liệu CSV:', parsedData);
    })
    .catch(error => {
        console.error('Có lỗi xảy ra khi tải file CSV:', error);
    });

    // Gọi hàm xử lý dữ liệu ở đây
    initGearControl();
    initSignalControl();
    updateTime();
    updateBatteryIcon(20);
    odoDisplay.update(0);
});